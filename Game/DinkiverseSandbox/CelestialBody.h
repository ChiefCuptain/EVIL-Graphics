#pragma once
#include "Framework/Actor.h"
#include "Constants.h"
#include "Components/Box2DPhysicsComponent.h"
#include "Math/Vector2.h"
#include <list>

#define GET_GRAVITATIONAL_FORCE(m1, m2, distance) GRAV_CONSTANT * ( (m1 * m2) / ( distance * distance ) )

namespace nu
{
	class CelestialBody : public Actor
	{
		CLASS_PROTOTYPE(CelestialBody)
	public:
		CelestialBody() = default;
		~CelestialBody() = default;
		void Start() override;
		void Update(float dt) override;
		void Draw(const class Renderer& r) const;

		void OnCollision(Actor* other) override;

		bool IsAffected() const { return m_affected; }
		void SetAffected(bool affected = true) { m_affected = affected; }

		const std::list<Vector2>& GetTrailPoints() const { return m_trailPoints; }

		void GoUp(float force);

		const Vector2& GetStartVelocity() { return m_startVelocity; }
		void SetStartVelocity(const Vector2& velocity) { m_startVelocity = velocity; }

		const Vector2& GetVelocity() const { return m_physicsComponent->GetVelocity(); }

		void SetMass(float mass) { m_mass = mass; }
		float GetMass() const { return m_mass; }

		void Read(const json::value_t& value) override;
	private:
		std::list<Vector2> m_trailPoints;
		bool m_affected = false;
		Vector2 m_startVelocity;
		Box2DPhysicsComponent* m_physicsComponent = nullptr;
		float m_mass = 0.0f;
	};
}