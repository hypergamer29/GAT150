#include "Enemy.h"
#include "Player.h"
#include "Pew.h"
#include "SpaceGame.h"
#include "Framework/Scene.h"
#include "Renderer/Renderer.h"
#include "Audio/AudioSystem.h"
#include "Framework/Framework.h"

bool Enemy::Initialize() {
	Actor::Initialize();

	// cache off
	auto collisionComponent = GetComponent<Loki::CollisionComponent>();
	if (collisionComponent) {
		auto renderComponent = GetComponent<Loki::RenderComponent>();
		if (renderComponent) {
			float scale = transform.scale;
			collisionComponent->m_radius = renderComponent->GetRadius() * scale;
		}
	}

	return true;
}

void Enemy::Update(float dt) {
	Actor::Update(dt);

	Loki::vec2 forward = Loki::vec2{ 0, -1 }.Rotate(transform.rotation);
	Player* player =  m_scene->GetActor<Player>();
	if (player) {
		Loki::Vector2 directionY = (player->transform.position - 10) - transform.position;

		//turn twards player
		//float turnAngle = Loki::vec2::SignedAngle(forward, direction.Normalized());
		//m_transform.rotation += turnAngle * dt;

		//check if player is in front
		//if (std::fabs(turnAngle) < Loki::DegToRad(30.0f)) {

		//}
	}

	transform.position += forward * m_speed * Loki::g_time.GetDeltaTime();

	//Loop round the screen
	transform.position.x = Loki::Wrap(transform.position.x, (float)Loki::g_renderer.GetWidth());
	transform.position.y = Loki::Wrap(transform.position.y, (float)Loki::g_renderer.GetHeight());

	m_fireTimer -= (float)dt;

	if (m_fireTimer <= 0.0f) {
		/*
		//create pew
		Loki::Transform transform1(transform.position, transform.rotation + Loki::DegToRad(10.0f), 1.25f);
		std::unique_ptr<Pew> pew = std::make_unique<Pew>(400.0f, transform1);
		pew->tag = "EnemyBullet";

		std::unique_ptr<Loki::SpriteComponent> component = std::make_unique<Loki::SpriteComponent>();
		component->m_texture = GET_RESOURCE(Loki::Texture, "pew.png", Loki::g_renderer);
		pew->AddComponent(std::move(component));

		m_scene->Add(std::move(pew));
		m_fireTimer = 5.0f;
		*/
	}
}

void Enemy::OnCollision(Actor* other) {
	if (other->tag == "PlayerBullet") {
  		SetLifeForce(GetLifeForce() - 5);
		if (GetLifeForce() <= 0) m_destroyed = true;

		Loki::SpaceGame score;
		Loki::EventManager::Instance().DispatchEvent("OnAddPoints", 1);
		//m_game->AddPoints(1);
		
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

		auto emitter = std::make_unique<Loki::Emitter>(transform, data);

		emitter->lifespan = 0.5f;
		m_scene->Add(std::move(emitter));
	}
}
