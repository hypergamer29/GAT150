#pragma once
#include "Framework/Actor.h"

namespace Loki {
	class Pew : public Actor {
	public:
		CLASS_DECLARATION(Pew)

		Pew() = default;
		
		bool Initialize() override;

		void Update(float dt) override;
		void OnCollision(Actor* other);

	private:
		float speed = 0;

	};
}