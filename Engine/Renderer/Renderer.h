#pragma once
#include "MaterialInstance.h"
#include "Uniforms.h"
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>
namespace nu {
	class Renderer
	{
	public:
		~Renderer();
		bool Initialize(const char* name, int width, int height);
		void Quit();


		bool BeginFrame();
		bool EndFrame();

		void SetVertexUniform(UniformSlot slot, const void* data, uint32_t size) const;
		void SetFragmentUniform(UniformSlot slot, const void* data, uint32_t size) const;

		void SetFragmentTexture(uint32_t slot, const class Texture& texture);

		float GetWindowWidth() const { return m_screen_size.x; }
		float GetWindowHeight() const { return m_screen_size.y; }

		SDL_GPUDevice* GetGPUDevice() const { return m_gpu_device; }
		SDL_Window* GetWindow() const { return m_window; }

		void SetPipeline(const class Pipeline& pipeline) const;
		void SetVertexBuffer(const class VertexBuffer& vertexBuffer) const;
		void SetIndexBuffer(const class IndexBuffer& indexBuffer) const;
		void Draw(uint32_t vertexCount);
		void DrawIndex(const class IndexBuffer& indexBuffer) const;

		void SetMaterial(const MaterialInstance& materialInstance) const;
		
		void EndRenderPass();
		SDL_GPUCommandBuffer* GetGPUCommandBuffer() const { return m_command_buffer; }
		SDL_GPUTexture* GetGPUSwapchainTexture() const { return m_swapchainTexture; }

		void CreateDepthBuffer();

		friend class IndexBuffer;
		friend class VertexBuffer;
		friend class Texture;
		friend class Text;
		friend class Shader;

	private:

		SDL_GPUDevice* m_gpu_device = nullptr;
		SDL_GPUCommandBuffer* m_command_buffer = nullptr;
		SDL_GPURenderPass* m_render_pass = nullptr;
		SDL_GPUTexture* m_depthTexture = nullptr;
		SDL_GPUTexture* m_swapchainTexture = nullptr;

		SDL_Window* m_window = nullptr;
		glm::vec2 m_screen_size{ 0, 0 };
	};
}
