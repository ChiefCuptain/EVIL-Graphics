#pragma once
#include "Framework/Game.h"
#include "Resources/ResourceManager.h"
#include "Math/Vector2.h"

class Scene;
namespace nu
{
	class DinkiverseSandbox : public Game
	{
	public:
		enum class GameState
		{
			Main,
			Paused,
		};
	public:
		DinkiverseSandbox() = default;

		bool Initialize() override;

		void Update(float dt) override;

		void Draw(nu::Renderer& renderer) override;

		void PlanetCreation();

		void CreatePlanet(const Vector2& pos, const Vector2& vel);

	private:
		GameState m_gamestate = DinkiverseSandbox::GameState::Main;
		int m_selected = 1;
		bool m_creating = false;
		Vector2 m_create_position{ 0.0f };
	}; 
}