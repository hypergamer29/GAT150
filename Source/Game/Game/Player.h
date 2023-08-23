#pragma once
#include "Framework/Actor.h"
#include "Framework/Components/PhysicsComponent.h"

class Player : public Loki::Actor {
public:
	Player() = default;
	Player(float speed, float turnRate, const Loki::Transform& transform) :
		Actor{ transform },
		m_speed{ speed },
		m_turnRate{ turnRate }
	{};

	bool Initialize() override;
	void Update(float dt) override;
	void OnCollision(Actor* other) override;

	int GetLifeForce() { return m_lifeForce; }
	void SetLifeForce(int lifeForce) { m_lifeForce = lifeForce; }
private:
	float m_speed = 0;
	float m_turnRate = 0;
	int m_lifeForce = 20;

	class Loki::PhysicsComponent* m_physicsComponent = nullptr;
};