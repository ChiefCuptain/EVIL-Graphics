#include "pch.h"
#include "DinkiverseSandbox.h"
#include "Framework/Scene.h"
#include "Resources/ResourceManager.h"
#include "CelestialBody.h"
#include "Components/Box2DPhysicsComponent.h"


bool nu::DinkiverseSandbox::Initialize()
{
	SetWorkingDirectory("DinkiverseSandbox");

	Game::Initialize();

	m_scene = std::make_unique<Scene>();
	m_scene->Load("Scenes/scene.json");
	nu::Engine::Get().GetAudio().AddSound("planet_explode", "Audio/snd_planet_ded.wav");
	nu::Engine::Get().GetAudio().AddSound("clear_all", "Audio/snd_clear_all.wav");
	nu::Resources().GetWithID<nu::Texture>("small_crosshair", "Textures/spr_crosshair_small.png", nu::Engine::Get().GetRenderer());
	nu::Resources().GetWithID<nu::Texture>("medium_crosshair", "Textures/spr_crosshair_medium.png", nu::Engine::Get().GetRenderer());
	nu::Resources().GetWithID<nu::Texture>("pause_symbol", "Textures/spr_pause_symbol.png", nu::Engine::Get().GetRenderer());
	nu::Resources().GetWithID<nu::Texture>("launch_crosshair", "Textures/spr_launch_crosshair.png", nu::Engine::Get().GetRenderer());
	return true;
}

void nu::DinkiverseSandbox::Update(float dt)
{
	nu::Vector2 mouse_pos = nu::Engine::Get().GetInput().GetMousePosition();
	if (nu::Engine::Get().GetInput().GetKeyPressed(SDL_SCANCODE_1))
	{
		m_selected = 1;
	}
	if (nu::Engine::Get().GetInput().GetKeyPressed(SDL_SCANCODE_2))
	{
		m_selected = 2;
	}
	if (nu::Engine::Get().GetInput().GetMousePressed(nu::Input::Left))
	{
		PlanetCreation();
	}
	if (nu::Engine::Get().GetInput().GetMousePressed(nu::Input::Right))
	{
		m_creating = false;
	}
	if (nu::Engine::Get().GetInput().GetKeyPressed(SDL_SCANCODE_DELETE))
	{
		m_scene->RemoveAllActors();
		Engine::Get().GetAudio().PlaySound("clear_all");
	}
	switch (m_gamestate)
	{
	case DinkiverseSandbox::GameState::Main:
		m_scene->Update(dt);
		if (nu::Engine::Get().GetInput().GetKeyPressed(SDL_SCANCODE_SPACE))
		{
			m_gamestate = DinkiverseSandbox::GameState::Paused;
		}
		break;
	case DinkiverseSandbox::GameState::Paused:
		if (nu::Engine::Get().GetInput().GetKeyPressed(SDL_SCANCODE_SPACE))
		{
			m_gamestate = DinkiverseSandbox::GameState::Main;
		}
		break;
	}
}

void nu::DinkiverseSandbox::Draw(nu::Renderer& renderer)
{ 
	m_scene->Draw(renderer);
	nu::Vector2 mouse_pos = nu::Engine::Get().GetInput().GetMousePosition();
	if (m_creating)
	{
		nu::Vector2 crosshair_pos = m_create_position + ((m_create_position - mouse_pos) * 0.4f);
		renderer.DrawTexture(*nu::Resources().GetWithID<nu::Texture>("launch_crosshair", "Textures/spr_launch_crosshair.png", nu::Engine::Get().GetRenderer()), crosshair_pos.x, crosshair_pos.y);
	}
	switch (m_selected)
	{
	case 1:
		renderer.DrawTexture(*nu::Resources().GetWithID<nu::Texture>("small_crosshair", "Textures/spr_crosshair_small.png", nu::Engine::Get().GetRenderer()), (m_creating) ? m_create_position.x : mouse_pos.x, (m_creating) ? m_create_position.y : mouse_pos.y);
		break;
	case 2:
		renderer.DrawTexture(*nu::Resources().GetWithID<nu::Texture>("medium_crosshair", "Textures/spr_crosshair_medium.png", nu::Engine::Get().GetRenderer()), (m_creating) ? m_create_position.x : mouse_pos.x, (m_creating) ? m_create_position.y : mouse_pos.y);
		break;
	}
	switch (m_gamestate)
	{
	case DinkiverseSandbox::GameState::Main:
		break;
	case DinkiverseSandbox::GameState::Paused:
		renderer.DrawTexture(*nu::Resources().GetWithID<nu::Texture>("pause_symbol", "Textures/spr_pause_symbol.png", nu::Engine::Get().GetRenderer()), 1856.0f, 64.0f);
		break;
	}
}

void nu::DinkiverseSandbox::PlanetCreation()
{
	if (m_creating)
	{
		Vector2 final_pos = nu::Engine::Get().GetInput().GetMousePosition();
		Vector2 final_vel{ 0.0f };

		final_vel = (final_pos - m_create_position) * -1 * 0.4f;

		CreatePlanet(m_create_position, final_vel);
		m_creating = false;
	}
	else
	{
		m_create_position = nu::Engine::Get().GetInput().GetMousePosition();
		m_creating = true;
	}
	
}

void nu::DinkiverseSandbox::CreatePlanet(const nu::Vector2& pos, const nu::Vector2& vel)
{
	std::unique_ptr<CelestialBody> actor;
	if (m_selected == 1)
	{
		actor = Factory::Instance().Create<nu::CelestialBody>("Planet_Small");
	}
	else if (m_selected == 2)
	{
		actor = Factory::Instance().Create<nu::CelestialBody>("Planet_Medium");
	}
	actor->SetPosition(pos);
	actor->SetStartVelocity(vel);
	m_scene->AddActor(std::move(actor));
}

