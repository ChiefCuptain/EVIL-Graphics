#pragma once
#include "RendererComponent.h"
#include "Renderer/Light.h"
namespace nu
{
	class LightComponent : public RendererComponent
	{
	public:
		CLASS_PROTOTYPE(LightComponent);

		void Draw(const Renderer& renderer) const override;

		LightData GetLightData() const;

		void Read(const json::value_t& value) override;

	private:
		LightType m_type = LightType::POINT;
		glm::vec3 m_color{ 1.0f };
		float m_intensity = 1.0f;
		float m_range = 20.0f;
		float m_innerAngle = 20.0f;
		float m_outerAngle = 30.0f;

	};
}
