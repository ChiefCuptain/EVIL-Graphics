#pragma once
#include "RendererComponent.h"
#include "Resources/ResourceManager.h"
#include "Renderer/Model.h"

namespace nu
{
	class ModelComponent : public RendererComponent
	{
	public:
		CLASS_PROTOTYPE(ModelComponent);

		void Draw(const Renderer& renderer) const override;
		void Read(const json::value_t& value) override;

	private:
		res_t<Model> m_model;
		std::vector<res_t<class MaterialInstance>> m_materials;
	};
}
