#pragma once
#include "Framework/Actor.h"

#include <vector>
#include <string>
#include <memory>

namespace nu
{
	class Scene
	{
	public:
		Scene() = default;
		void AddActor(std::unique_ptr<Actor> actor);

		bool Load(const std::string& sceneName);

		void Update(float dt);
		void Draw(const class Renderer& renderer);

		template<typename T>
		T* GetActorByName(const std::string& name);

		int GetActorCountByTag(const std::string& tag) const;

		template<typename T>
		std::vector<T*> GetActorsByType(const std::string& name);

		void RemoveAllActors(bool include_persistent = false);

		void ClearActors() { m_pending_clear = true; }
	

	private:
		bool m_pending_clear = false;
		std::vector<std::unique_ptr<Actor>> m_actors;
		std::vector<std::unique_ptr<Actor>> m_pending_actors;
	};

	template<typename T>
	inline T* Scene::GetActorByName(const std::string& name)
	{
		for (auto& actor : m_actors)
		{
			T* actorT = dynamic_cast<T*>(actor.get());
			if (actorT && actorT->m_name == name)
			{
				return actorT;
			}
		}
		
		return nullptr;
	}

	template<typename T>
	inline std::vector<T*> Scene::GetActorsByType(const std::string& name)
	{
		std::vector<T*> foundActors;
		for (auto& actor : m_actors)
		{
			T* actorT = dynamic_cast<T*>(actor.get());
			if (actorT && actorT->m_type == name)
			{
				foundActors.push_back(actorT);
			}
		}

		return foundActors;
	}
}