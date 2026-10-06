#include "pch.h"
#include "TextureFrames.h"
#include "Serialization/JSON.h"
#include "Renderer/Texture.h"
namespace nu
{
	bool TextureFrames::Load(const std::string& filename, class Renderer& r)
	{
		json::document_t document;
		if (!json::Load(filename, document)) {
			std::cerr << "Could not load TextureFrames : " << filename << "\n";
			return false;
		}
		
		std::string texture_name;
		JSON_READ_REQ(document, texture_name);
		if (texture_name.empty()) return false;

		m_texture = Resources().Get<Texture>(texture_name, r);
		if (!m_texture)
		{
			std::cerr << "Could not load TextureFrames.m_texture : " << filename << "\n";
			return false;
		}

		JSON_READ_NAME(document, "columns", m_columns);
		JSON_READ_NAME(document, "rows", m_rows);
		JSON_READ_NAME(document, "start_frame", m_startFrame);
		JSON_READ_NAME(document, "total_frames", m_totalFrames);

		Vector2 textureSize = m_texture->GetSize();

		if (m_columns <= 0 || m_rows <= 0)
		{
			std::cerr << "TextureFrames has a zero column or row size : " << filename << "\n";
			return false;
		}
		m_frameSize = textureSize / Vector2{ m_columns, m_rows };

		return true;
	}

	Rect TextureFrames::GetFrameRect(unsigned int frame)
	{
		if (frame >= m_totalFrames)
		{
			std::cerr << "Frame " << frame << " of TextureFrames is out of bounds.\n";
			
		}
		int currentFrame = m_startFrame + frame;
		int column = currentFrame % m_columns;
		int row = currentFrame / m_columns;

		float x = column * m_frameSize.x;
		float y = row * m_frameSize.y;

		return Rect{ x, y, m_frameSize.x, m_frameSize.y };
	}

}