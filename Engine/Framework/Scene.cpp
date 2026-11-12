#include "pch.h"

#include "Framework/Scene.h"
#include "Framework/Actor.h"
#include "Core/Factory.h"
#include "Engine.h"
#include "Renderer/Renderer.h"
#include "Renderer/Light.h"
#include "Components/CameraComponent.h"
#include "Components/LightComponent.h"


namespace nu
{

	bool Scene::Load(const std::string& sceneName)
	{
		json::document_t document;
		if (json::Load(sceneName, document))
		{
			if (JSON_HAS_NAME(document, "actors"))
			{
				for (auto& actorValue : JSON_GET_NAME(document, "actors").GetArray())
				{
					std::string typeName;
					JSON_READ_NAME(actorValue, "type", typeName);

					auto actor = Factory::Instance().Create<Actor>(typeName);
					actor->Read(actorValue);

					bool prototype = false;
					JSON_READ(actorValue, prototype);

					if (prototype)
					{
						std::string name;
						JSON_READ(actorValue, name);
						Factory::Instance().RegisterPrototype<Actor>(name, std::move(actor));
					}
					else
					{
						AddActor(std::move(actor));
					}
				}
			}
		}
		else
		{
			return false;
		}

		return true;
	}

	void Scene::AddActor(std::unique_ptr<Actor> actor)
	{
		actor->m_scene = this;
		m_pending_actors.push_back(std::move(actor));
	}

	void Scene::Update(float dt)
	{

		for (auto& actor : m_actors)
		{
			actor->Update(dt);
		}

		std::erase_if(m_actors, [](auto& actor) {
			if (actor->GetDestroyedQueued()) { actor->OnDestroy(); return true; }
			return false;
			});
		for (auto& actor : m_actors)
			if (actor->m_destroyed) actor->SetDestroyedQueued();


		for (auto& actor : m_pending_actors) {
			actor->Start();
			m_actors.push_back(std::move(actor));
		}
		m_pending_actors.clear();

		if (m_pending_clear)
		{
			RemoveAllActors();
		}
	}

	void Scene::Draw(const Renderer& renderer)
	{
		FrameUniform frameUniform{};

		auto camera = GetActorByName<Actor>("Camera");
		if (camera)
		{
			auto cameraComponent = camera->GetComponent<CameraComponent>();
			if (cameraComponent)
			{
				frameUniform.cameraPosition = camera->GetTransform().position;
				frameUniform.view = cameraComponent->GetView();
				frameUniform.projection = cameraComponent->GetProjection();
				frameUniform.viewProjection = frameUniform.projection * frameUniform.view;

				renderer.SetVertexUniform(UniformSlot::Frame, &frameUniform, sizeof(frameUniform));
				renderer.SetFragmentUniform(UniformSlot::Frame, &frameUniform, sizeof(frameUniform));
			}
		}

		LightUniform lightUniform{};
		lightUniform.ambient = glm::vec3{ 0.2f };

		auto lights = GetActorsByTag<Actor>("Light");
		for (auto light : lights)
		{
			auto lightComponent = light->GetComponent<LightComponent>();
			if (lightComponent)
			{
				lightUniform.lights[lightUniform.count++] = lightComponent->GetLightData();
			}
		}

		renderer.SetVertexUniform(UniformSlot::Light, &lightUniform, sizeof(lightUniform));
		renderer.SetFragmentUniform(UniformSlot::Light, &lightUniform, sizeof(lightUniform));

		for (const auto& actor : m_actors)
		{
			if (actor && actor->IsActive())
				actor->Draw(renderer);
		}
	}

	int Scene::GetActorCountByTag(const std::string& tag) const
	{
		int count = 0;
		for (const auto& actor : m_actors)
		{
			if (actor->GetTag() == tag) count += 1;
		}
		return count;
	}

	void Scene::RemoveAllActors(bool include_persistent)
	{
		m_pending_clear = false;
		if (include_persistent)
		{
			m_actors.clear();
		}
		else
		{
			std::erase_if(m_actors, [](auto& actor) { actor->OnDestroy(); return !actor->GetPersistent(); });

		}
	}
}