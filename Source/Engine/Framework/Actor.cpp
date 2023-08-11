#include "Framework/Actor.h"
#include "Components/RenderComponent.h"

namespace Loki {
	bool Actor::Initialize() {
		for (auto& component : m_components) {
			component->Initialize();
		}

		return true;
	}

	void Actor::OnDestroy() {
		for (auto& component : m_components) {
			component->OnDestroy();
		}
	}

	void Actor::Update(float dt) {
		if (m_lifespan != -1.0f) {
			m_lifespan -= dt;
			m_destroyed = (m_lifespan <= 0);
		}
		for (auto& component : m_components) {
			component->Update(dt);
		}
	}

	void Actor::Draw(Loki::Renderer& renderer) {
		for (auto& component : m_components) {

			if (dynamic_cast<RenderComponent*>(component.get())) {
				dynamic_cast<RenderComponent*>(component.get())->Draw(renderer);

			}
		}
	}

	void Actor::AddComponent(std::unique_ptr<Components> component) {
		component->m_owner = this;
		m_components.push_back(std::move(component));
	}
}