#pragma once

#include "Core/Random.h"
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Transform.h"

#include "Framework/Actor.h"
#include "Framework/Scene.h"
#include "Renderer/Texture.h"
#include "Core/File.h"
#include "Serialization/JSON.h"
#include "Core/StringUtils.h"

#include "Core/Factory.h"
#include "Resources/ResourceManager.h"

#include "Renderer/Renderer.h"
#include "Renderer/Text.h"
#include "Input/Input.h"
#include "Core/GameTime.h"
#include "Timer.h"
#include "Math/MathUtil.h"
#include "Renderer/ParticleSystem.h"
#include "Audio/Audio.h"
#include "Physics/Physics.h"

#include "Framework/Game.h"

#include <iostream>
#include <vector>
#include <string>

namespace nu
{
	class Engine
	{
	public:
		static Engine& Get() { static Engine engine; return engine; }

		bool Initialize();
		void Quit();

		void Update();

		Input& GetInput() { return m_input; }
		Renderer& GetRenderer() { return m_renderer; }
		GameTime& GetTime() { return m_time; }
		ParticleSystem& GetPS() { return m_particle_system; }
		Audio& GetAudio() { return m_audio; }
		Physics& GetPhysics() { return m_physics; }
		Engine(const Engine&) = delete;
		Engine& operator = (const Engine&) = delete;

	private:
		Engine() = default;
	private:
		Input m_input;
		Renderer m_renderer;
		Audio m_audio;
		Physics m_physics;
		GameTime m_time;
		ParticleSystem m_particle_system;
	};

}