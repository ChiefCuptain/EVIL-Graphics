#pragma once
#include "Font.h"

struct SDL_Texture;

namespace nu
{
	class Renderer;
	struct Vector3;

	class Text {
	public:
		Text() = default;
		Text(res_t<Font> font) : m_font{ font } {}
		~Text();

		bool Create(Renderer& renderer, const std::string& text, const Vector3& color);
		void Draw(Renderer& renderer, float x, float y, bool centered = true);

	private:
		res_t<Font> m_font;
		SDL_Texture* m_texture{ nullptr };
	};
}