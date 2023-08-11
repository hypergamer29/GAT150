#pragma once
#include "Components.h"

namespace Loki {
	class CollisionComponent : public Components{
	public:
		virtual bool CheckCollision(CollisionComponent* collision) = 0;

	public:
		float m_radius = 0;

	};
}