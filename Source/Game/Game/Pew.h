#pragma once
#include "Framework/Actor.h"

class Pew : public Loki::Actor {
public:
	Pew() = default;
	Pew(float speed, const Loki::Transform& transform) :
		Actor{ transform },
		m_speed{ speed }
	{ m_lifespan = 2.0f; }

	void Update(float dt) override;
	void OnCollision(Actor* other) override;

private:
	float m_speed = 0;
	float m_turnRate = 0;

};