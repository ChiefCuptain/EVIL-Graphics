#pragma once
#include "Components/Component.h"
namespace nu
{
	class RendererComponent : public Component
	{
	public:
		virtual void Draw(const class Renderer& r) const = 0;
	};
}