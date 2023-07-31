#pragma once
#include "Framework/Actor.h"

class Player : public Loki::Actor {
public:
	Player() = default;
	Player(float speed, float turnRate, const Loki::Transform& transform, std::shared_ptr<Loki::Model> model) :
		Actor{ transform, model },
		m_speed{ speed },
		m_turnRate{ turnRate }
	{};

	void Update(float dt) override;
	void OnCollision(Actor* other) override;

	int GetLifeForce() { return m_lifeForce; }

private:
	float m_speed = 0;
	float m_turnRate = 0;
	int m_lifeForce = 20;

};