#include "pch.h"
#include "Renderer/Renderer.h"
#include "Math/Transform.h"
#include "Renderer/Model.h"
#include "Math/MathUtil.h"
#include "Renderer/Texture.h"
#include "Math/Rect.h"
#include "Renderer/Pipeline.h"
#include "Renderer/VertexBuffer.h"
#include "IndexBuffer.h"

namespace nu
{
    void Renderer::Quit()
    {
        TTF_Quit();
        SDL_DestroyRenderer(m_renderer);
        SDL_DestroyWindow(m_window);
        SDL_Quit();
    }

    bool Renderer::Initialize(const char* name, int width, int height)
    {
        m_screen_size.x = (float)width;
        m_screen_size.y = (float)height;

        SDL_Init(SDL_INIT_VIDEO);

        if (!TTF_Init()) {
            std::cerr << "TTF_Init Error: " << SDL_GetError() << std::endl;
            return false;
        }

        m_window = SDL_CreateWindow(name, width, height, 0);
        if (m_window == nullptr) {
            std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
            SDL_Quit();
            return false;
        }
        SDL_GPUShaderFormat formats = SDL_GPU_SHADERFORMAT_SPIRV | SDL_GPU_SHADERFORMAT_DXIL;
        m_gpu_device = SDL_CreateGPUDevice(formats, true, NULL);

        SDL_ClaimWindowForGPUDevice(m_gpu_device, m_window);
        
        std::cout << "GPU Device : " << SDL_GetGPUDeviceDriver(m_gpu_device) << "\n";

        CreateDepthBuffer();

        return true;
    }

    void Renderer::SetColor(Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha) const
    {
        SDL_SetRenderDrawColor(m_renderer, red, green, blue, alpha);
    }

    void Renderer::SetColorFloat(float red, float green, float blue, float alpha) const
    {
        SDL_SetRenderDrawColorFloat(m_renderer, red, green, blue, alpha);
    }

    void Renderer::Clear() {
        SDL_RenderClear(m_renderer);
    }

    bool Renderer::BeginFrame()
    {
        m_command_buffer = SDL_AcquireGPUCommandBuffer(m_gpu_device);
        if (!m_command_buffer)
        {
            std::cerr << "Could not acquire command buffer: " << SDL_GetError() << std::endl;
            return false;
        }


        SDL_GPUTexture* swapchainTexture = nullptr;
        if (!SDL_WaitAndAcquireGPUSwapchainTexture(m_command_buffer, m_window, &swapchainTexture, nullptr, nullptr))
        {
            std::cerr << "Could not acquire swapchain texture: " << SDL_GetError() << std::endl;
            return false;
        }

        if (swapchainTexture != nullptr)
        {
            // configure the color target attachments (This handles clearing the screen)
            SDL_GPUColorTargetInfo color_target_info{};
            color_target_info.texture = swapchainTexture;
            color_target_info.clear_color = SDL_FColor{ 0.3f, 0.3f, 0.3f, 1.0f };
            color_target_info.load_op = SDL_GPU_LOADOP_CLEAR;
            color_target_info.store_op = SDL_GPU_STOREOP_STORE;
            // configure the depth target (This handles the depth sorting)
            SDL_GPUDepthStencilTargetInfo depth_target_info{
                .texture = m_depthTexture,
                .clear_depth = 1.0f,
                .load_op = SDL_GPU_LOADOP_CLEAR,
                .store_op = SDL_GPU_STOREOP_DONT_CARE,
                .stencil_load_op = SDL_GPU_LOADOP_DONT_CARE,
                .stencil_store_op = SDL_GPU_STOREOP_DONT_CARE,
            };
            m_render_pass = SDL_BeginGPURenderPass(m_command_buffer, &color_target_info, 1, &depth_target_info);
        }

        return true;
    }

    bool Renderer::EndFrame()
    {
        if (m_render_pass)
        {
            SDL_EndGPURenderPass(m_render_pass);
            m_render_pass = nullptr;
        }

        if (!SDL_SubmitGPUCommandBuffer(m_command_buffer))
        {
            std::cerr << "Could not submit command buffer: " << SDL_GetError() << std::endl;
            return false;
        }

        m_command_buffer = nullptr;

        return true;
    }

    void Renderer::SetVertexUniform(uint32_t slot, const void* data, uint32_t size)
    {
        SDL_PushGPUVertexUniformData(m_command_buffer, slot, data, size);
    }

    void Renderer::SetFragmentUniform(uint32_t slot, const void* data, uint32_t size)
    {
        SDL_PushGPUFragmentUniformData(m_command_buffer, slot, data, size);

    }

    void Renderer::RenderPresent() const
    { 
        SDL_RenderPresent(m_renderer);
    }

    void Renderer::RenderPoint(float x, float y) const
    {
        SDL_RenderPoint(m_renderer, x, y);
    }

    void Renderer::RenderLine(float x1, float y1, float x2, float y2) const
    {
        SDL_RenderLine(m_renderer, x1, y1, x2, y2);
    }

    void Renderer::RenderRect(float x, float y, float width, float height, bool fill = true) const
    {
        SDL_FRect rect{ x, y, width, height };
        (fill) ? SDL_RenderFillRect(m_renderer, &rect) : SDL_RenderRect(m_renderer, &rect);
    }

    void Renderer::SetFragmentTexture(uint32_t slot, const Texture& texture)
    {
        SDL_GPUTextureSamplerBinding binding
        {
            .texture = texture.m_gpuTexture,
            .sampler = texture.m_gpuSampler
        };

        SDL_BindGPUFragmentSamplers(m_render_pass, slot, &binding, 1);
    }


    void Renderer::SetPipeline(const Pipeline& pipeline)
    {
        SDL_BindGPUGraphicsPipeline(m_render_pass, pipeline.m_gpuPipeline);
    }

    void Renderer::SetVertexBuffer(const VertexBuffer& vertexBuffer)
    {
        // describe which buffer to bind and where to start reading from
        SDL_GPUBufferBinding binding{
            .buffer = vertexBuffer.m_gpuBuffer , // todo: the vertex buffer's gpu buffer
            .offset = 0   // start reading at the beginning of the buffer
        };

        // bind the vertex buffer to the current render pass with SDL_BindGPUVertexBuffers()
        // the draw calls that follow read their vertices from this buffer
        // parameters:
        //   render pass   - the current render pass (m_renderPass)
        //   first slot    - 0, matches the buffer slot set in Pipeline::AddVertexBuffer()
        //   bindings      - pointer to the binding above
        //   binding count - 1, we are binding one buffer

        SDL_BindGPUVertexBuffers(
            m_render_pass,
            0,
            &binding,
            1
        );
    }

    void Renderer::SetIndexBuffer(const IndexBuffer& indexBuffer)
    {
        // describe which buffer to bind and where to start reading from
        SDL_GPUBufferBinding binding{
            .buffer = indexBuffer.m_gpuBuffer , // todo: the index buffer's gpu buffer
            .offset = 0   // start reading at the beginning of the buffer
        };

        // bind the vertex buffer to the current render pass with SDL_BindGPUVertexBuffers()
        // the draw calls that follow read their vertices from this buffer
        // parameters:
        //   render pass   - the current render pass (m_renderPass)
        //   bindings      - pointer to the binding above
        //   index value size - size of the elements in the buffer

        SDL_BindGPUIndexBuffer(
            m_render_pass,
            &binding,
            SDL_GPU_INDEXELEMENTSIZE_32BIT
        );
    }

    void Renderer::Draw(uint32_t vertexCount)
    {
        // draw using the currently bound pipeline and vertex buffer with SDL_DrawGPUPrimitives()
        // parameters:
        //   render pass    - the current render pass (m_renderPass)
        //   vertex count   - the number of vertices to draw (vertexCount)
        //   instance count - 1, draw one copy (more than 1 is used for instancing)
        //   first vertex   - 0, start at the first vertex in the buffer
        //   first instance - 0, start at the first instance
        SDL_DrawGPUPrimitives(
            m_render_pass,
            vertexCount,
            1,
            0,
            0
        );
    }

    void Renderer::DrawIndex(const IndexBuffer& indexBuffer)
    {
        SDL_DrawGPUIndexedPrimitives(
            m_render_pass,
            indexBuffer.GetIndexCount(),
            1,
            0,
            0,
            0
        );
    }

    void Renderer::CreateDepthBuffer()
    {
        SDL_GPUTextureCreateInfo depthInfo
        {
            .type = SDL_GPU_TEXTURETYPE_2D,
            .format = SDL_GPU_TEXTUREFORMAT_D32_FLOAT,
            .usage = SDL_GPU_TEXTUREUSAGE_DEPTH_STENCIL_TARGET,
            .width = (Uint32)GetWindowWidth(),
            .height = (Uint32)GetWindowHeight(),
            .layer_count_or_depth = 1,
            .num_levels = 1,
            .sample_count = SDL_GPU_SAMPLECOUNT_1,
        };

        m_depthTexture = SDL_CreateGPUTexture(m_gpu_device, &depthInfo);
    }
    
    Renderer::~Renderer()
    {
        SDL_DestroyRenderer(m_renderer);
        SDL_DestroyWindow(m_window);
        SDL_Quit();
    }
};