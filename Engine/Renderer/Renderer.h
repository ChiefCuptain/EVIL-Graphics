#pragma once
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

		void SetColor(Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha = 255) const;
		void SetColorFloat(float red, float green, float blue, float alpha = 1.0f) const;

		void Clear();

		bool BeginFrame();
		bool EndFrame();

		void SetVertexUniform(UniformSlot slot, const void* data, uint32_t size) const;
		void SetFragmentUniform(UniformSlot slot, const void* data, uint32_t size) const;

		void RenderPresent() const;
		void RenderPoint(float x, float y) const;
		void RenderLine(float x1, float y1, float x2, float y2) const;
		void RenderRect(float x, float y, float width, float height, bool fill) const;

		void SetFragmentTexture(uint32_t slot, const class Texture& texture);

		float GetWindowWidth() const { return m_screen_size.x; }
		float GetWindowHeight() const { return m_screen_size.y; }

		SDL_GPUDevice* GetGPUDevice() const { return m_gpu_device; }
		SDL_Window* GetWindow() const { return m_window; }

		void SetPipeline(const class Pipeline& pipeline);
		void SetVertexBuffer(const class VertexBuffer& vertexBuffer);
		void SetIndexBuffer(const class IndexBuffer& indexBuffer);
		void Draw(uint32_t vertexCount);
		void DrawIndex(const class IndexBuffer& indexBuffer);

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

		SDL_Window* m_window = nullptr;
		SDL_Renderer* m_renderer = nullptr;
		glm::vec2 m_screen_size{ 0, 0 };
	};
}
