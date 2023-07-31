#include "Player.h"
#include "Pew.h"
#include "Framework/Scene.h"
#include "Input/InputSystem.h"
#include "Audio/AudioSystem.h"

void Player::Update(float dt) {
	Actor::Update(dt);

	//Rotation
	float rotate = 0;
	if (Loki::g_inputSystem.GetKeyDown(SDL_SCANCODE_A)) rotate = -1;
	if (Loki::g_inputSystem.GetKeyDown(SDL_SCANCODE_D)) rotate = 1;
	m_transform.rotation += rotate * m_turnRate * Loki::g_time.GetDeltaTime();

	//Moving
	float thrust = 0;
	if (Loki::g_inputSystem.GetKeyDown(SDL_SCANCODE_W)) thrust = 1;

	Loki::vec2 forward = Loki::vec2{ 0, -1 }.Rotate(m_transform.rotation);
	AddForce(forward * m_speed * thrust);
	
	//m_transform.position += forward * m_speed * thrust * Loki::g_time.GetDeltaTime();
	
	//Loop round the screen
	m_transform.position.x = Loki::Wrap(m_transform.position.x, (float)Loki::g_renderer.GetWidth());
	m_transform.position.y = Loki::Wrap(m_transform.position.y, (float)Loki::g_renderer.GetHeight());

	//Fire Pew
	if (Loki::g_inputSystem.GetKeyDown(SDL_SCANCODE_SPACE) && !Loki::g_inputSystem.GetPreviousKeyDown(SDL_SCANCODE_SPACE)) {

		//create pew
		Loki::Transform transform1(m_transform.position, m_transform.rotation, 0.5f);
		std::unique_ptr<Pew> pew = std::make_unique<Pew>(400.0f, transform1, m_model);
		pew->m_tag = "PlayerBullet";
		m_scene->Add(std::move(pew));
 		Loki::g_audioSystem.PlayOneShot("Pew");
		
		Loki::Transform transform2(m_transform.position, m_transform.rotation - Loki::DegToRad(10.0f), 0.5f);
		pew = std::make_unique<Pew>(400.0f, transform2, m_model);
		pew->m_tag = "PlayerBullet";
		m_scene->Add(std::move(pew));
	}

	if (Loki::g_inputSystem.GetKeyDown(SDL_SCANCODE_T)) Loki::g_time.SetTImeScale(0.5f);
	else Loki::g_time.SetTImeScale(1.0f);
}

void Player::OnCollision(Actor* other) {
	if (other->m_tag == "EnemyBullet") {
		int life = GetLifeForce();
		life -= 5;
		if (life <= 0) m_destroyed = true;
	}

}
