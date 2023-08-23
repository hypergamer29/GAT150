#pragma once
#include "Object.h"
#include "Core/Core.h"
#include "Renderer/Moedel.h"
#include "Components/Components.h"
#include <memory>

namespace Loki {
	class Actor : public Object {
	public:
		CLASS_DECLARATION(Actor)

		Actor() = default;
		Actor(const Loki::Transform& transform) :
			transform{ transform }
		{}
		Actor(const Actor& other);

		virtual bool Initialize() override;
		virtual void OnDestroy() override;

		virtual void Update(float dt);
		virtual void Draw(Loki::Renderer& renderer);

		void AddComponent(std::unique_ptr<Components> component);
		template<typename T>
		T* GetComponent();

		float GetRadius() { return 30.0f; }
		virtual void OnCollision(Actor* other) {}

		class Scene* m_scene = nullptr;
		friend class Scene;
		friend class model;

		Transform transform;
		std::string tag;
		class Game* m_game = nullptr;
		
		float lifespan = -1.0f;
		bool persistent = false;
		bool prototype = false;

	protected:
		std::vector<std::unique_ptr<class Components>> components;

		bool m_destroyed = false;
	};


	template<typename T>
	inline T* Actor::GetComponent() {
		for (auto & component : components) {
			T* result = dynamic_cast<T*>(component.get());
			if (result) return result;

		}
		return nullptr;
	}
}