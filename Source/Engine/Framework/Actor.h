#pragma once
#include "Core/Core.h"
#include "Renderer/Moedel.h"
#include <memory>

namespace Loki {
	class Actor {
	public:
		Actor() = default;
		Actor(const Loki::Transform& transform, std::shared_ptr<Model> model) :
			m_transform{ transform },
			m_model{ model }
		{}

		Actor(const Loki::Transform& transform) :
			m_transform{ transform }
		{}

		virtual void Update(float dt);
		virtual void Draw(Loki::Renderer& renderer);

		float GetRadius() { return (m_model) ? m_model->GetRadius() * m_transform.scale : 0; }
		virtual void OnCollision(Actor* other) {}

		void AddForce(const vec2 force) { m_velocity += force; }
		void SetDamping(float damping) { m_damping = damping; }

		class Scene* m_scene = nullptr;
		friend class Scene;
		friend class model;

		Transform m_transform;
		std::string m_tag;
		
		float m_lifespan = -1.0f;

		class Game* m_game = nullptr;

	protected:
		bool m_destroyed = false;

		std::shared_ptr<Model> m_model;

		vec2 m_velocity;
		float m_damping = 0;
	};
}