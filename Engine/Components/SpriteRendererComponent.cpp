#include "pch.h"
#include "SpriteRendererComponent.h"
#include "Renderer/Renderer.h"
#include "Framework/Actor.h"
#include "Resources/ResourceManager.h"
#include "Engine.h"

namespace nu
{
	FACTORY_REGISTER(SpriteRendererComponent);

	void SpriteRendererComponent::Start()
	{
		if (!m_textureName.empty())
		{
			m_texture = Resources().Get<Texture>(m_textureName, Engine::Get().GetRenderer());
			if (m_texture)
			{
				m_size = m_texture->GetSize();
			}
		}
	}

	void SpriteRendererComponent::Draw(const Renderer& r) const
	{
		if (m_texture)
		{
			if (m_sourceRect.w > 0 && m_sourceRect.h > 0)
			{
				r.DrawTexture(*m_texture, m_sourceRect, GetOwner()->GetTransform(), m_flipH, m_flipV);
			}
			else
			{
			r.DrawTexture(*m_texture, GetOwner()->GetTransform(), m_flipH, m_flipV);
			}
		}
	}

	void SpriteRendererComponent::Read(const json::value_t& value)
	{
		RendererComponent::Read(value);

		JSON_READ_NAME(value, "texture", m_textureName);
		JSON_READ_NAME(value, "flip_h", m_flipH);
		JSON_READ_NAME(value, "flip_v", m_flipV);

	}
}