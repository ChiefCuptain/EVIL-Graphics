#pragma once
#include <fmod.hpp>
#include <fmod_errors.h>
#include <string>

namespace nu
{
	class Audio
	{
	public:
		Audio() = default;

		bool Initialize();
		void Quit();

		void Update();

		bool AddSound(const std::string& name, const char * filename);
		bool PlaySound(const std::string& name);

	private:
		bool CheckFMODResult(FMOD_RESULT result);

	private:
		FMOD::System* m_fmodSystem = nullptr;
		std::map<std::string, FMOD::Sound*> m_sounds;
	};
}