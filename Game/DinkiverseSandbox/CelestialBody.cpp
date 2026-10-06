#include "pch.h"
#include "CelestialBody.h"
#include "Core/Factory.h"
#include "Framework/Scene.h"
#include "Renderer/Renderer.h"
#include "Renderer/ParticleSystem.h"
#include "Core/Random.h"
#include "Resources/ResourceManager.h"
#include "Engine.h"

namespace nu
{
	FACTORY_REGISTER(CelestialBody)


	void CelestialBody::Start()
	{
		Actor::Start();
		m_physicsComponent = GetComponent<Box2DPhysicsComponent>();
		if (m_physicsComponent)
		{
			m_physicsComponent->Start();
			m_mass = m_physicsComponent->GetMass() * 10000.0f;
		}
	}

	void CelestialBody::Update(float dt)
	{
		Actor::Update(dt);
		if (m_trailPoints.size() == 0)
		{
			m_trailPoints.push_back(GetTransform().position);
		}
		else if (GetTransform().position.Distance(m_trailPoints.back()) > 10.0f) 
		{
			if (m_trailPoints.size() >= 250)
			{
				m_trailPoints.pop_front();
			}
			m_trailPoints.push_back(GetTransform().position);
		}
		

		if (m_physicsComponent)
		{
			if (IsAffected())
			{
				const auto& bodies = m_scene->GetActorsByType<CelestialBody>("CelestialBody");

				for (auto& body : bodies)
				{
					if (body == this)
					{
						continue;
					}
					float forceMagnitude = (float)(GET_GRAVITATIONAL_FORCE(m_mass, body->GetMass(), (body->GetTransform().position - m_transform.position).Length()));
					Vector2 force = m_transform.position.DirectionTo(body->GetTransform().position) * forceMagnitude;
					m_physicsComponent->ApplyForce(force);
				}
			}
		}
	}
	
	void CelestialBody::Draw(const Renderer& r) const
	{
		if (m_affected)
		{
			r.SetColor(119, 124, 129);
			Vector2 prevPoint{};
			for (auto& point : m_trailPoints)
			{
				if (point == m_trailPoints.front())
				{
					prevPoint = point;
					continue;
				}
				Vector2 p1 = prevPoint;
				Vector2 p2 = point;
				r.RenderLine(prevPoint.x, prevPoint.y, point.x, point.y);
				prevPoint = point;
			}
		}
		Actor::Draw(r);
	}

	void CelestialBody::OnCollision(Actor* other)
	{
		if (other->GetDestroyed() || GetDestroyed()) return;
		if (other->GetTag() == "CelestialBody")
		{

			auto otherBody = dynamic_cast<CelestialBody*>(other);
			if (otherBody)
			{
				Engine::Get().GetAudio().PlaySound("planet_explode");
				if (otherBody->GetMass() >= GetMass())
				{
					for (int i = 0; i < 24; i++)
					{
						nu::Particle particle;
						particle.position = m_transform.position;
						particle.color = { 1.0f, 1.0f, 1.0f };
						particle.lifespan = RandomFloat(1.0f, 2.0f);
						particle.velocity = { RandomFloat(-300.0f, 300.0f), RandomFloat(-300.0f, 300.0f) };
						particle.angularVelocity = RandomFloat(-45.0f, 45.0f);

						Engine::Get().GetPS().AddParticle(particle, Resources().GetWithID<nu::Texture>("explosion", "Textures/spr_planet_chunk.png", Engine::Get().GetRenderer()));
					}
					SetDestroyed();
				}
				else
				{
					for (int i = 0; i < 24; i++)
					{
						nu::Particle particle;
						particle.position = otherBody->GetTransform().position;
						particle.color = { 1.0f, 1.0f, 1.0f };
						particle.lifespan = RandomFloat(1.0f, 2.0f);
						particle.velocity = { RandomFloat(-300.0f, 300.0f), RandomFloat(-300.0f, 300.0f) };
						particle.angularVelocity = RandomFloat(-45.0f, 45.0f);

						Engine::Get().GetPS().AddParticle(particle, Resources().GetWithID<nu::Texture>("explosion", "Textures/spr_planet_chunk.png", Engine::Get().GetRenderer()));
					}
				}
			}
		}
	}

	void CelestialBody::Read(const json::value_t& value)
	{
		Actor::Read(value);

		JSON_READ_NAME_REQ(value, "is_affected", m_affected);
	}
}