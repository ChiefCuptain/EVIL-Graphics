#include "pch.h"
#include "SpriteAnimationRendererComponent.h"
#include "Core/Factory.h"
#include "Resources/ResourceManager.h"
#include "Renderer/TextureFrames.h"
#include "Engine.h"

namespace nu
{
	FACTORY_REGISTER(SpriteAnimationRendererComponent);

	void SpriteAnimationRendererComponent::Start()
	{
		if (!m_textureFramesName.empty())
		{
			m_textureFrames = Resources().Get<TextureFrames>(m_textureFramesName, Engine::Get().GetRenderer());
			if (m_textureFrames)
			{
				m_sourceRect = m_textureFrames->GetFrameRect(0);
				m_size = Vector2{ m_sourceRect.w, m_sourceRect.h };
				m_texture = m_textureFrames->GetTexture();
			}
			if (!m_textureFrames)
			{
				std::cerr << "Could not load TextureFrames : " << m_textureFramesName << "\n";
			}
		}
	}

	void SpriteAnimationRendererComponent::Update(float dt)
	{
		if (!m_textureFrames) return;
		m_frameTimer += dt;
		float frameTime = 1.0f / m_fps;

		while (m_frameTimer >= frameTime)
		{
			m_frame++;
			if (m_loop)
			{
				m_frame = Wrap(0u, m_textureFrames->GetTotalFrames() - 1, m_frame);
			}
			else
			{
				m_frame = Clamp(0u, m_textureFrames->GetTotalFrames() - 1, m_frame);
			}


			m_frameTimer -= frameTime;
		}

		m_sourceRect = m_textureFrames->GetFrameRect(m_frame);
	}

	void SpriteAnimationRendererComponent::Read(const json::value_t& value)
	{
		SpriteRendererComponent::Read(value);

		JSON_READ_NAME_REQ(value, "fps", m_fps);
		JSON_READ_NAME(value, "loop", m_loop);

		JSON_READ_NAME_REQ(value, "texture_frames", m_textureFramesName);

	}
}
