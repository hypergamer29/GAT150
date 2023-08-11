#pragma once
#include "RenderComponent.h"
#include "Renderer/Moedel.h"

namespace Loki {
	class ModelRenderComponent : public RenderComponent {
	public:
		void Update(float dt) override;
		void Draw(class Renderer& renderer) override;

	public:
		res_t<Model> m_model;
	};
}