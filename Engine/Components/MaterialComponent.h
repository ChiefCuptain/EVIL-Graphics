#pragma once
#include "RendererComponent.h"

namespace nu
{
	class MaterialComponent : public RendererComponent
	{
	public:
		CLASS_PROTOTYPE(MaterialComponent);

		void Draw(const Renderer& renderer) const override;

	private:
	};
}
