#include "pch.h"
#include "ParticleSystem.h"
#include "Renderer/Renderer.h"

namespace nu
{
	bool ParticleSystem::Initialize(size_t size)
	{
		// reserve # particles in container
		m_particles.resize(size);

		return true;
	}

	void ParticleSystem::Quit()
	{
		// clear all particles
		m_particles.clear();
	}

	void ParticleSystem::Update(float dt)
	{
		// update active particles
		for (auto& particle : m_particles)
		{
			if (!particle.active) continue;
			particle.lifespan -= dt;
			particle.active = particle.lifespan > 0;
			particle.position += particle.velocity * dt;
			particle.rotation += particle.angularVelocity * dt;
		}
	}

	void ParticleSystem::Draw(const Renderer& renderer)
	{
		// draw all active particle
		for (auto& particle : m_particles)
		{
			if (particle.active)
			{
				if (particle.texture)
				{
					renderer.DrawTexture(*particle.texture, particle.position.x, particle.position.y, 0.0f, 0.5f);
				}
				else
				{
					renderer.SetColorFloat(particle.color.r, particle.color.g, particle.color.b);
					renderer.RenderPoint(particle.position.x, particle.position.y);
				}
				
			}
		}
	}

	void ParticleSystem::AddParticle(const Particle& particle)
	{
		// get free particle
		Particle* freeParticle = GetFreeParticle();
		// check if free particle is not nullptr
		if (freeParticle)
		{
			// set free particle object with particle
			*freeParticle = particle;

			// set particle active
			// TODO: set free particle active to true
			freeParticle->active = true;
		}
	}

	void ParticleSystem::AddParticle(const Particle& particle, res_t<nu::Texture> texture)
	{
		// get free particle
		Particle* freeParticle = GetFreeParticle();
		// check if free particle is not nullptr
		if (freeParticle)
		{
			*freeParticle = particle;
			freeParticle->texture = texture;
			freeParticle->active = true;
		}
	}

	Particle* ParticleSystem::GetFreeParticle()
	{
		// find and return inactive particle 
		for (auto& particle : m_particles)
		{
			// return pointer to inactive particle
			if (!particle.active)
				return &particle;
		}

		// no free particles, return nullptr
		return nullptr;
	}
}