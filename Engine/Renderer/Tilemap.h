#pragma once
#include "Resources/ResourceManager.h"
#include "Math/Rect.h"
#include "Math/Vector2.h"
#include <vector>
namespace nu
{
	class Tilemap : public Resource
	{
	public:
		struct Layer
		{
			int width = 0;
			int height = 0;
			bool hasCollision = false;
			std::vector<int> data;
			res_t<class Texture> texture;
		};
	public:
		bool Load(const std::string& filename, class Renderer& r);

		const std::vector<Layer>& GetLayers() const { return m_layers; }
		Rect GetTileRect(const Layer& layer, int tileID);
		Vector2 GetTilePosition(const Layer& layer, int tileIndex);
	private:
		int m_tileWidth = 0;
		int m_tileHeight = 0;

		std::vector<Layer> m_layers;
	};


}