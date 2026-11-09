#include "pch.h"
#include "CameraControllerComponent.h"
#include "Engine.h"
#include "Framework/Actor.h"

namespace nu
{
	FACTORY_REGISTER(CameraControllerComponent);

	void CameraControllerComponent::Update(float dt)
	{
		// get camera rotation
		if (Engine::Get().GetInput().GetMouseDown(Input::MouseButton::Right))
		{
			glm::vec2 axis = Engine::Get().GetInput().GetMouseDelta() * m_sensitivity;

			m_yaw -= axis.x;
			m_pitch -= axis.y;
			m_pitch = glm::clamp(m_pitch, -89.0f, 89.0f);
		}

		//owner->transform.SetRotation(glm::vec3(pitch, yaw, 0.0f));

		//glm::quat q = glm::quat_cast(glm::eulerAngleYXZ(glm::radians(m_yaw), glm::radians(m_pitch), 0.0f));
		//m_owner->GetTransform().SetRotationQuat(q);

		m_owner->SetRotation(glm::vec3(m_pitch, m_yaw, 0.0f));

		glm::vec3 direction{ 0 };
		if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_A)) direction -= m_owner->GetTransform().Right();
		if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_D)) direction += m_owner->GetTransform().Right();
		if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_E)) direction += m_owner->GetTransform().Up();
		if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_Q)) direction -= m_owner->GetTransform().Up();
		if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_W)) direction += m_owner->GetTransform().Forward();
		if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_S)) direction -= m_owner->GetTransform().Forward();;

		if (glm::length2(direction) > 0.0f) {
			direction = glm::normalize(direction);
		}
		m_owner->SetPosition(m_owner->GetTransform().position + direction * 4.0f * dt);

		/*glm::vec3 direction{ 0 };
		if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_A)) direction.x = -1;
		if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_D)) direction.x = 1;
		if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_W)) direction.z = -1;
		if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_S)) direction.z = 1;
		if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_Q)) direction.y = -1;
		if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_E)) direction.y = 1;
		m_owner->SetPosition(m_owner->GetTransform().position + direction * 4.0f * dt);*/
	}

	void CameraControllerComponent::Read(const json::value_t value)
	{
		Component::Read(value);
		JSON_READ_NAME(value, "camera_speed", m_cameraSpeed);
		JSON_READ_NAME(value, "sensitivity", m_sensitivity);
	}
}