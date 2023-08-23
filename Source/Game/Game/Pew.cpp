#include "Pew.h"
#include "Renderer/Renderer.h"
#include "Framework/Framework.h"

namespace Loki {
	CLASS_DEFINITION(Pew)

	bool Pew::Initialize() {
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

	void Pew::Update(float dt) {
		Actor::Update(dt);

		Loki::vec2 forward = Loki::vec2{ 0, -1 }.Rotate(transform.rotation);
		transform.position += forward * speed * Loki::g_time.GetDeltaTime();

		//Loop round the screen
		transform.position.x = Loki::Wrap(transform.position.x, (float)Loki::g_renderer.GetWidth());
		transform.position.y = Loki::Wrap(transform.position.y, (float)Loki::g_renderer.GetHeight());

	}

	void Pew::OnCollision(Actor* other) {
		if ("PlayerBullet" == "Enemy") {
			m_destroyed = true;
		}

		if ("EnemyBullet" == "Player") {
			m_destroyed = true;
		}
	}

	void Pew::Read(const json_t& value) {
		Actor::Read(value);
		READ_DATA(value, speed)
	}
}