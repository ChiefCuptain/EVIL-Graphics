#include "pch.h"
#include "Audio.h"



namespace nu
{
	bool Audio::Initialize()
	{
		FMOD_RESULT result = FMOD::System_Create(&m_fmodSystem);
		if (!CheckFMODResult(result))
			return false;

		void* extradriverdata = nullptr;
		result = m_fmodSystem->init(32, FMOD_INIT_NORMAL, extradriverdata);
		if (!CheckFMODResult(result))
			return false;

		return true;
	}

	void Audio::Quit()
	{
		CheckFMODResult(m_fmodSystem->release());
	}

	void Audio::Update()
	{
		CheckFMODResult(m_fmodSystem->update());
	}

	bool Audio::AddSound(const std::string& name, const char * filename)
	{
		// check if key exists in sounds map
		if (m_sounds.contains(name))
		{
			std::cerr << "Audio System : name already exists " << name << std::endl;
			return false;
		}

		// create sound from key
		FMOD::Sound* sound = nullptr;
		FMOD_RESULT result = m_fmodSystem->createSound(filename, FMOD_DEFAULT, 0, &sound);
		if (!CheckFMODResult(result))
			return false;

		// insert sound into map
		//TODO: add sound to m_sounds using name as key

		m_sounds[name] = sound;
		return true;
	}

	bool Audio::PlaySound(const std::string& name)
	{
		// check if sound exists in sounds map
		if (!m_sounds.contains(name))
		{
			std::cerr << "Audio System : name doesn't exists " << name << std::endl;
			return false;
		}

		// play sound from key
		FMOD_RESULT result = m_fmodSystem->playSound(m_sounds[name], nullptr, false, nullptr);
		if (!CheckFMODResult(result))
			return false;

		return true;
	}

	bool Audio::CheckFMODResult(FMOD_RESULT result)
	{
		if (result != FMOD_OK)
		{
			std::cerr << FMOD_ErrorString(result) << std::endl;
			return false;
		}

		return true;
	}
}