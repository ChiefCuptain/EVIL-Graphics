#include "pch.h"
#include "SpriteAnimatorRendererComponent.h"
#include "Renderer/TextureFrames.h"
#include "Math/MathUtil.h"
#include "Core/StringUtils.h"
#include "Engine.h"
namespace nu
{
	FACTORY_REGISTER(SpriteAnimatorRendererComponent)
	void SpriteAnimatorRendererComponent::Start()
	{
		if (!m_defaultAnimName.empty())
		{
			Play(m_defaultAnimName);
		}
		else
		{
			auto iter = m_spriteAnims.begin();
			const std::string name = iter->first;
			Play(name);
		}
	}

	void SpriteAnimatorRendererComponent::Update(float dt)
	{
		if (!m_currentAnim.textureFrames) return;
		m_frameTimer += dt;
		float frameTime = 1.0f / m_currentAnim.fps;

		while (m_frameTimer >= frameTime)
		{
			m_frame++;
			if (m_currentAnim.loop)
			{
				m_frame = Wrap(0u, m_currentAnim.textureFrames->GetTotalFrames() - 1, m_frame);
			}
			else
			{
				m_frame = Clamp(0u, m_currentAnim.textureFrames->GetTotalFrames() - 1, m_frame);
			}


			m_frameTimer -= frameTime;
		}

		m_sourceRect = m_currentAnim.textureFrames->GetFrameRect(m_frame);
	}

	void SpriteAnimatorRendererComponent::Play(const std::string& name)
	{
		auto iter = m_spriteAnims.find(string::ToLower(name));
		if (iter == m_spriteAnims.end())
		{
			std::cerr << "Could not find animation : " << name << "\n";
			return;
		}
		m_currentAnim = iter->second;

		m_frame = 0;
		m_frameTimer = 0.0f;
		
		m_texture = m_currentAnim.textureFrames->GetTexture();
		m_sourceRect = m_currentAnim.textureFrames->GetFrameRect(m_frame);
	}

	void SpriteAnimatorRendererComponent::Read(const json::value_t& value)
	{
		SpriteRendererComponent::Read(value);

		JSON_READ_NAME(value, "default_animation", m_defaultAnimName);

		if (JSON_HAS(value, animations) && JSON_GET(value, animations).IsArray())
		{
			for (auto& animatorValue : JSON_GET(value, animations).GetArray())
			{
				SpriteAnimation spriteAnimation;

				std::string textureAnimationName;

				JSON_READ_NAME(animatorValue, "name", spriteAnimation.name);

				std::string texture_frames;
				JSON_READ_REQ(animatorValue, texture_frames);

				if (!texture_frames.empty())
				{
					spriteAnimation.textureFrames = Resources().Get<TextureFrames>(texture_frames, Engine::Get().GetRenderer());
					if (!spriteAnimation.textureFrames)
					{
						std::cerr << "Could not load texture frames: " << texture_frames << std::endl;
					}
				}

				JSON_READ_NAME_REQ(animatorValue, "fps", spriteAnimation.fps);
				JSON_READ_NAME(animatorValue, "loop", spriteAnimation.loop);

				m_spriteAnims[string::ToLower(spriteAnimation.name)] = spriteAnimation;
			}
		}

	}

}