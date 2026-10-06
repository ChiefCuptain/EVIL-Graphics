#pragma once
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>
#include "Math/Vector2.h"

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

		void RenderPresent() const;
		void RenderPoint(float x, float y) const;
		void RenderLine(float x1, float y1, float x2, float y2) const;
		void RenderRect(float x, float y, float width, float height, bool fill) const;

		void DrawModel(const class Model& model, const class Transform& transform) const;
		void DrawTexture(const class Texture& texture, float x, float y, float angle = 0.0f, float scale = 1.0f, bool flipH = false, bool flipV = false) const;
		void DrawTexture(const class Texture& texture, const class Transform& transform, bool flipH = false, bool flipV = false) const;
		void DrawTexture(const class Texture& texture, const struct Rect& source, float x, float y, float angle = 0.0f, float scale = 1.0f, bool flipH = false, bool flipV = false) const;
		void DrawTexture(const class Texture& texture, const struct Rect& source, const class Transform& transform, bool flipH = false, bool flipV = false) const;

		float GetWindowWidth() const { return m_screen_size.x; }
		float GetWindowHeight() const { return m_screen_size.y; }

		const Vector2& GetCamera() { return m_camera; }
		void SetCamera(const Vector2& camera) { m_camera = camera; }
		bool GetCameraEnabled() const { return m_camera_enabled; }
		void SetCameraEnabled(bool enabled = true) { m_camera_enabled = enabled; }

		friend class Texture;
		friend class Text;
	private:

		Vector2 m_camera{ 0.0f };
		bool m_camera_enabled = false;
		SDL_Window* m_window = nullptr;
		SDL_Renderer* m_renderer = nullptr;
		Vector2 m_screen_size{ 0, 0 };
	};
}
