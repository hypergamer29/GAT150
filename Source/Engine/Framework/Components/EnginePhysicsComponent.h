#pragma once
#include "PhysicsComponent.h"

namespace Loki {

	class EnginePhysicsComponent : public PhysicsComponent{
	public:
		CLASS_DECLARATION(EnginePhysicsComponent);

		virtual void Update(float dt) override;
		virtual void ApplyForce(const vec2& force) override;

	};
}