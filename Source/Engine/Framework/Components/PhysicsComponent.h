#pragma once
#include "Components.h"
#include "Core/Vector2.h"


namespace Loki {
	class PhysicsComponent : public Components {
	public:
		virtual void ApplyForce(const vec2& force);

	public:
		vec2 m_velocity;
		vec2 m_acceleration;
		float m_mass = 1.0f;
		float m_damping = 0.0f;
	};
}