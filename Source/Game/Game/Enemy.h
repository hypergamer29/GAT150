#pragma once
#include "Framework/Actor.h"

class Enemy : public Loki::Actor {
public:
	Enemy() = default;
	Enemy(float speed, float turnRate, const Loki::Transform& transform) :
		Actor{ transform },
		m_speed{ speed },
		m_turnRate{ turnRate }
	{
		m_fireRate = 2.0f;
		m_fireTimer = m_fireRate;
	};

	bool Initialize() override;

	void Update(float dt) override;
	void OnCollision(Actor* other) override;

	int GetLifeForce() { return m_lifeForce; }
	void SetLifeForce(int lifeForce) { m_lifeForce = lifeForce; }

private:
	float m_speed = 0;
	float m_turnRate = 0;

	float m_fireRate = 0;
	float m_fireTimer = 0;

	int m_lifeForce = 10;
};