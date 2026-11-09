#pragma once
#include "Component.h"

namespace nu
{
	class CameraControllerComponent : public Component
	{
	public:
		CLASS_PROTOTYPE(CameraControllerComponent);

		void Update(float dt) override;

		float GetCameraSpeed() const { return m_cameraSpeed; }
		void SetCameraSpeed(float cameraSpeed) { m_cameraSpeed = cameraSpeed; }

		void Read(const json::value_t value);

	private:
		float m_cameraSpeed = 1.0f;
		float m_sensitivity = 1.0f;
		float m_yaw = 0.0f;
		float m_pitch = 0.0f;
	};
}