#pragma once
#include "Components.h"

namespace Loki {
	class RenderComponent : public Components {
	public:
		virtual void Draw(class Renderer& renderer) = 0;
		virtual float GetRadius() { return 0; }
	};
}