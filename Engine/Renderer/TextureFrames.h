#pragma once
#include "Resources/ResourceManager.h"
#include "Math/Vector2.h"
#include "Math/Rect.h"
#include <string>
namespace nu
{
	class TextureFrames : public Resource
	{
	public:

		bool Load(const std::string& filename, class Renderer& r);
		Rect GetFrameRect(unsigned int frame);

		unsigned int GetTotalFrames() const { return m_totalFrames; }
		res_t<class Texture> GetTexture() { return m_texture; }
	private:
		unsigned int m_columns = 0;
		unsigned int m_rows = 0;
		unsigned int m_startFrame = 0;
		unsigned int m_totalFrames = 0;

		Vector2 m_frameSize{ 0.0f };

		res_t<class Texture> m_texture;
	};
}