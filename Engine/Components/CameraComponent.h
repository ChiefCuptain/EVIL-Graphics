#pragma once
#include "RendererComponent.h"

namespace nu
{
	class CameraComponent : public RendererComponent
	{
	public:
		CLASS_PROTOTYPE(CameraComponent);

		void Update(float dt) override;

		void Draw(const Renderer& renderer) const override;

		void Read(const json::value_t& value) override;

		void SetFOV(float fov) { m_fov = fov; }
		void GetFOV(float fov) { m_fov = fov; }
		void SetNearPlane(float nearPlane) { m_nearPlane = nearPlane; }
		void GetNearPlane(float nearPlane) { m_nearPlane = nearPlane; }
		void SetFarPlane(float farPlane) { m_farPlane = farPlane; }
		void GetFarPlane(float farPlane) { m_farPlane = farPlane; }

	private:
		glm::mat4x4 m_view;
		glm::mat4x4 m_projection;

		float m_fov = 70.0f;
		float m_nearPlane = 0.001f;
		float m_farPlane = 250.0f;
	};
}
