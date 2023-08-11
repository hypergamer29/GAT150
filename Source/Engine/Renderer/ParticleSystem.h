#pragma once
#include "Particle.h"
#include "Core/Math/Vector2.h"
#include <vector>


namespace Loki {
	class ParticleSystem {
	public:
		ParticleSystem() = default;
		ParticleSystem(int maxCount) {
			m_particles.resize(maxCount);
		}

		void Update(float dt);
		void Draw(Renderer& renderer);

		Particle* GetFreeParticle();

		
	private:
		std::vector<Particle> m_particles;
	};

	extern ParticleSystem g_particleSystem;
}