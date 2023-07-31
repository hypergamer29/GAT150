#include "Enemy.h"
#include "Player.h"
#include "Pew.h"
#include "SpaceGame.h"
#include "Framework/Scene.h"
#include "Renderer/Renderer.h"
#include "Audio/AudioSystem.h"
#include "Framework/Emitter.h"


void Enemy::Update(float dt) {
	Actor::Update(dt);

	Loki::vec2 forward = Loki::vec2{ 0, -1 }.Rotate(m_transform.rotation);
	Player* player =  m_scene->GetActor<Player>();
	if (player) {
		Loki::Vector2 directionY = (player->m_transform.position - 10) - m_transform.position;

		//turn twards player
		//float turnAngle = Loki::vec2::SignedAngle(forward, direction.Normalized());
		//m_transform.rotation += turnAngle * dt;

		//check if player is in front
		//if (std::fabs(turnAngle) < Loki::DegToRad(30.0f)) {

		//}
	}

	m_transform.position += forward * m_speed * Loki::g_time.GetDeltaTime();

	//Loop round the screen
	m_transform.position.x = Loki::Wrap(m_transform.position.x, (float)Loki::g_renderer.GetWidth());
	m_transform.position.y = Loki::Wrap(m_transform.position.y, (float)Loki::g_renderer.GetHeight());

	m_fireTimer -= (float)dt;

	if (m_fireTimer <= 0.0f) {
		//create pew
		Loki::Transform transform1(m_transform.position, m_transform.rotation + Loki::DegToRad(10.0f), 0.5f);
		std::unique_ptr<Pew> pew = std::make_unique<Pew>(400.0f, transform1, m_model);
		pew->m_tag = "EnemyBullet";
		m_scene->Add(std::move(pew));
		m_fireTimer = 5.0f;
	}
}

void Enemy::OnCollision(Actor* other) {
	if (other->m_tag == "PlayerBullet") {
  		SetLifeForce(GetLifeForce() - 5);
		if (GetLifeForce() <= 0) m_destroyed = true;

		Loki::SpaceGame score;
		m_game->AddPoints(1);
		
		//create explostion
		Loki::EmitterData data;
		data.burst = true;
		data.burstCount = 100;
		data.spawnRate = 200;
		data.angle = 0;
		data.angleRange = Loki::Pi;
		data.lifetimeMin = 0.1f;
		data.lifetimeMax = 0.5f;
		data.speedMin = 50;
		data.speedMax = 250;
		data.damping = 0.5f;
		data.color = Loki::Color{ 1, 0, 0, 1 };

		auto emitter = std::make_unique<Loki::Emitter>(m_transform, data);

		emitter->m_lifespan = 0.5f;
		m_scene->Add(std::move(emitter));
	}
}
