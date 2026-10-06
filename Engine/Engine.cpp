// Engine.cpp : Defines the functions for the static library.
//

#include "pch.h"
#include "Engine.h"
#include "framework.h"
// TODO: This is an example of a library function

namespace nu
{

	bool Engine::Initialize()
	{
		m_renderer.Initialize("Dinkiverse Sandbox", 1920, 1080);
		m_particle_system.Initialize(500);
		m_input.Initialize();
		m_audio.Initialize();
		m_physics.Initialize();

		return true;
	}

	void Engine::Quit()
	{
		m_physics.Quit();
		m_audio.Quit();
		m_input.Quit();
		m_particle_system.Quit();
		m_renderer.Quit();
	}

	void Engine::Update()
	{
		m_time.Tick();
		m_audio.Update();
		m_input.Update();
		m_particle_system.Update(m_time.GetDeltaTime());
	}
}
