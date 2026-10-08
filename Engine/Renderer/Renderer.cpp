#include "pch.h"
#include "Renderer/Renderer.h"
#include "Math/Transform.h"
#include "Renderer/Model.h"
#include "Math/MathUtil.h"
#include "Renderer/Texture.h"
#include "Math/Rect.h"
#include "Renderer/Pipeline.h"
#include "Renderer/VertexBuffer.h"

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
            color_target_info.clear_color = SDL_FColor{ 1.0f, 0.0f, 0.0f, 1.0f };
            color_target_info.load_op = SDL_GPU_LOADOP_CLEAR;
            color_target_info.store_op = SDL_GPU_STOREOP_STORE;

            m_render_pass = SDL_BeginGPURenderPass(m_command_buffer, &color_target_info, 1, nullptr);
            SDL_EndGPURenderPass(m_render_pass);
        }

        return true;
    }

    bool Renderer::EndFrame() const
    {
        if (!SDL_SubmitGPUCommandBuffer(m_command_buffer))
        {
            std::cerr << "Could not submit command buffer: " << SDL_GetError() << std::endl;
            return false;
        }

        return true;
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

    void Renderer::DrawModel(const Model& model, const Transform& transform) const
    {
        for (auto& mesh : model.GetMeshes()) {
            SetColorFloat(mesh.GetColor().r, mesh.GetColor().g, mesh.GetColor().b, 1.0f);
            auto& points = mesh.GetPoints();
            for (int i = 0; i < points.size(); ++i)
            {
                if (i == 0) { continue; }
                Vector2 v1 = points.at(i - 1);
                Vector2 v2 = points.at(i);

                v1 *= transform.scale;
                v2 *= transform.scale;

                v1 = v1.Rotate(transform.rotation * DegToRad);
                v2 = v2.Rotate(transform.rotation * DegToRad);

                v1 += transform.position;
                v2 += transform.position;

                RenderLine(v1.x, v1.y, v2.x, v2.y);
            }
        }
        
    }


    void Renderer::DrawTexture(const Texture& texture, float x, float y, float angle, float scale, bool flipH, bool flipV) const
    {
        Vector2 size = texture.GetSize();

        float cameraX = (m_camera_enabled) ? m_camera.x - GetWindowWidth() * 0.5f : 0.0f;
        float cameraY = (m_camera_enabled) ? m_camera.y - GetWindowHeight() * 0.5f : 0.0f;

        SDL_FRect destRect;
        destRect.w = size.x * scale;
        destRect.h = size.y * scale;
        destRect.x = (x - cameraX) - (destRect.w * 0.5f);
        destRect.y = (y - cameraY) - (destRect.h * 0.5f);

        // https://wiki.libsdl.org/SDL3/SDL_RenderTexture
        SDL_FlipMode flip_type = SDL_FLIP_NONE;
        if (flipH && flipV)
        {
            flip_type = SDL_FLIP_HORIZONTAL_AND_VERTICAL;
        }
        else if (flipH)
        {
            flip_type = SDL_FLIP_HORIZONTAL;
        }
        else if (flipV)
        {
            flip_type = SDL_FLIP_VERTICAL;
        }
        SDL_RenderTextureRotated(m_renderer, texture.m_texture, NULL, &destRect, angle, NULL, flip_type);
    }

    void Renderer::DrawTexture(const Texture& texture, const Transform& transform, bool flipH, bool flipV) const
    {
        Vector2 size = texture.GetSize();
        float cameraX = (m_camera_enabled) ? m_camera.x - GetWindowWidth() * 0.5f : 0.0f;
        float cameraY = (m_camera_enabled) ? m_camera.y - GetWindowHeight() * 0.5f : 0.0f;

        SDL_FRect destRect;
        destRect.w = size.x * transform.scale;
        destRect.h = size.y * transform.scale;


        destRect.x = (transform.position.x + cameraX) - (destRect.w * 0.5f);
        destRect.y = (transform.position.y + cameraY) - (destRect.h * 0.5f);

        // https://wiki.libsdl.org/SDL3/SDL_RenderTexture
        SDL_FlipMode flip_type = SDL_FLIP_NONE;
        if (flipH && flipV)
        {
            flip_type = SDL_FLIP_HORIZONTAL_AND_VERTICAL;
        }
        else if (flipH)
        {
            flip_type = SDL_FLIP_HORIZONTAL;
        }
        else if (flipV)
        {
            flip_type = SDL_FLIP_VERTICAL;
        }
        SDL_RenderTextureRotated(m_renderer, texture.m_texture, NULL, &destRect, transform.rotation, NULL, flip_type);
    }

    void Renderer::DrawTexture(const Texture& texture, const Rect& source, float x, float y, float angle, float scale, bool flipH, bool flipV) const
    {
        float cameraX = (m_camera_enabled) ? m_camera.x - GetWindowWidth() * 0.5f : 0.0f;
        float cameraY = (m_camera_enabled) ? m_camera.y - GetWindowHeight() * 0.5f : 0.0f;

        SDL_FRect sourceRect;
        sourceRect.x = source.x;
        sourceRect.y = source.y;
        sourceRect.w = source.w;
        sourceRect.h = source.h;

        SDL_FRect destRect;
        destRect.w = source.w * scale;
        destRect.h = source.h * scale;
        destRect.x = (x + cameraX) - (destRect.w * 0.5f);
        destRect.y = (y + cameraY) - (destRect.h * 0.5f);

        // https://wiki.libsdl.org/SDL3/SDL_RenderTexture
        SDL_FlipMode flip_type = SDL_FLIP_NONE;
        if (flipH && flipV)
        {
            flip_type = SDL_FLIP_HORIZONTAL_AND_VERTICAL;
        }
        else if (flipH)
        {
            flip_type = SDL_FLIP_HORIZONTAL;
        }
        else if (flipV)
        {
            flip_type = SDL_FLIP_VERTICAL;
        }
        SDL_RenderTextureRotated(m_renderer, texture.m_texture, &sourceRect, &destRect, angle, NULL, flip_type);
    }

    void Renderer::DrawTexture(const Texture& texture, const Rect& source, const Transform& transform, bool flipH, bool flipV) const
    {

        float cameraX = (m_camera_enabled) ? m_camera.x - GetWindowWidth() * 0.5f : 0.0f;
        float cameraY = (m_camera_enabled) ? m_camera.y - GetWindowHeight() * 0.5f : 0.0f;

        SDL_FRect sourceRect;
        sourceRect.x = source.x;
        sourceRect.y = source.y;
        sourceRect.w = source.w;
        sourceRect.h = source.h;

        SDL_FRect destRect;
        destRect.w = source.w * transform.scale;
        destRect.h = source.h * transform.scale;

        destRect.x = (transform.position.x + cameraX) - (destRect.w * 0.5f);
        destRect.y = (transform.position.y + cameraY) - (destRect.h * 0.5f);


        // https://wiki.libsdl.org/SDL3/SDL_RenderTexture
        SDL_FlipMode flip_type = SDL_FLIP_NONE;
        if (flipH && flipV)
        {
            flip_type = SDL_FLIP_HORIZONTAL_AND_VERTICAL;
        }
        else if (flipH)
        {
            flip_type = SDL_FLIP_HORIZONTAL;
        }
        else if (flipV)
        {
            flip_type = SDL_FLIP_VERTICAL;
        }
        SDL_RenderTextureRotated(m_renderer, texture.m_texture, &sourceRect, &destRect, transform.rotation, NULL, flip_type);
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

    Renderer::~Renderer()
    {
        SDL_DestroyRenderer(m_renderer);
        SDL_DestroyWindow(m_window);
        SDL_Quit();
    }
};