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
		m_renderer.Initialize("EVIL Graphics", 1920, 1080);
		m_input.Initialize();
		m_audio.Initialize();

		return true;
	}

	void Engine::Quit()
	{
		m_audio.Quit();
		m_input.Quit();
		m_renderer.Quit();
	}

	void Engine::Update()
	{
		m_time.Tick();
		m_audio.Update();
		m_input.Update();
	}
}
