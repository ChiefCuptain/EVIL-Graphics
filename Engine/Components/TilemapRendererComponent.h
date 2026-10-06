#pragma once
#include "RendererComponent.h"
#include "Core/Factory.h"
#include "Resources/ResourceManager.h"
namespace nu
{
	class TileMapRendererComponent : public RendererComponent
	{
	public:
		CLASS_PROTOTYPE(TileMapRendererComponent)

		TileMapRendererComponent() = default;
		TileMapRendererComponent(const TileMapRendererComponent& other);

		void Start() override;
		void Draw(const Renderer& r) const override;

		void Read(const json::value_t& value) override;
	private:
		std::string m_tilemapName;
		res_t<class Tilemap> m_tilemap;

		std::vector<std::unique_ptr<class PhysicsBody>> m_physicsBodies;
	};


}