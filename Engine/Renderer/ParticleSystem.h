#pragma once
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Resources/Resource.h"
#include <vector>

namespace nu
{
	class Texture;
	struct Particle
	{
		bool active = false;
		float lifespan = 1.0f;

		Vector2 position{ 0, 0 };
		float rotation = 0.0f;
		Vector2 velocity{ 0, 0 };
		float angularVelocity = 0.0f;
		Color color{ 0, 0, 0 };
		res_t<Texture> texture;
	};

	class ParticleSystem
	{
	public:
		ParticleSystem() = default;

		bool Initialize(size_t poolSize = 1000);
		void Quit();

		void Update(float dt);
		void Draw(const class Renderer& renderer);

		void AddParticle(const Particle& particle);
		void AddParticle(const Particle& particle, res_t<nu::Texture> texture);

	private:
		Particle* GetFreeParticle();

	private:
		// store particles in particle pool
		std::vector<Particle> m_particles;
	};
}