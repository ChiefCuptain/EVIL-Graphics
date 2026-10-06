#include "pch.h"
#include "GameTime.h"

namespace nu 
{
	GameTime::GameTime()
	{
		Reset();
	}

	void GameTime::Reset()
	{
		m_startTicks = SDL_GetTicksNS();
		m_frameTicks = SDL_GetTicksNS();
		m_currentTicks = SDL_GetTicksNS();
	}

	void GameTime::Tick()
	{
		m_currentTicks = SDL_GetTicksNS();
		m_deltaTicks = m_currentTicks - m_frameTicks;
		m_frameTicks = m_currentTicks;
	}


}