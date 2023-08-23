#include "Player.h"
#include "Pew.h"
#include "Framework/Framework.h"
#include "Input/InputSystem.h"
#include "Audio/AudioSystem.h"
#include "Renderer/Texture.h"
#include "Renderer/Renderer.h"

bool Player::Initialize() {
	Actor::Initialize();

	// cache off
	m_physicsComponent = GetComponent<Loki::PhysicsComponent>();
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

void Player::Update(float dt) {
	Actor::Update(dt);

	//Rotation
	float rotate = 0;
	if (Loki::g_inputSystem.GetKeyDown(SDL_SCANCODE_A)) rotate = -1;
	if (Loki::g_inputSystem.GetKeyDown(SDL_SCANCODE_D)) rotate = 1;
	transform.rotation += rotate * m_turnRate * Loki::g_time.GetDeltaTime();

	//Moving
	float thrust = 0;
	if (Loki::g_inputSystem.GetKeyDown(SDL_SCANCODE_W)) thrust = 1;

	Loki::vec2 forward = Loki::vec2{ 0, -1 }.Rotate(transform.rotation);
	
	m_physicsComponent->ApplyForce(forward * m_speed * thrust);
	
	//m_transform.position += forward * m_speed * thrust * Loki::g_time.GetDeltaTime();
	
	//Loop round the screen
	transform.position.x = Loki::Wrap(transform.position.x, (float)Loki::g_renderer.GetWidth());
	transform.position.y = Loki::Wrap(transform.position.y, (float)Loki::g_renderer.GetHeight());

	//Fire Pew
	if (Loki::g_inputSystem.GetKeyDown(SDL_SCANCODE_SPACE) && !Loki::g_inputSystem.GetPreviousKeyDown(SDL_SCANCODE_SPACE)) {
		auto pew = INSTANTIATE(Pew, "Pew");
		pew->transform = { transform.position, transform.rotation + Loki::DegToRad(10.0f), 1.5f};
		pew->Initialize();
		m_scene->Add(std::move(pew));

		
	}

	if (Loki::g_inputSystem.GetKeyDown(SDL_SCANCODE_T)) Loki::g_time.SetTImeScale(0.5f);
	else Loki::g_time.SetTImeScale(1.0f);
}

void Player::OnCollision(Actor* other) {
	if (other->tag == "EnemyBullet" || other->tag == "Enemy") {
		m_destroyed = true;
		Loki::EventManager::Instance().DispatchEvent("OnPlayerDead", 0);


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
