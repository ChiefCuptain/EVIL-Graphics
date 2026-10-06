#pragma once
#include "RendererComponent.h"
#include "Resources/Resource.h"
#include "Renderer/Texture.h"
#include "Math/Rect.h"

#include <string>

namespace nu
{
	class SpriteRendererComponent : public RendererComponent
	{
	public:
		CLASS_PROTOTYPE(SpriteRendererComponent)

		void Start();

		void Draw(const Renderer& r) const override;

		const Vector2& GetSize() const { return m_size; }

		bool GetFlipH() const { return m_flipH; }
		void SetFlipH(bool flip = true) { m_flipH = flip; }
		bool GetFlipV() const { return m_flipV; }
		void SetFlipV(bool flip = true) { m_flipV = flip; }

		virtual void Read(const json::value_t& value) override;
	protected:

		std::string m_textureName;
		Rect m_sourceRect;
		Vector2 m_size{ 0.0f };
		bool m_flipH = false;
		bool m_flipV = false;

		res_t<Texture> m_texture;
	};
}