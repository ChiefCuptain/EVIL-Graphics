#include "Bullet.h"
#include "Math/MathUtil.h"
#include "Core/Factory.h"
#include "Components/PhysicsComponent.h"

FACTORY_REGISTER(Bullet);


void Bullet::Update(float dt)
{
	nu::Vector2 forward{ 1.0f, 0.0f };
	nu::Vector2 velocity = forward.Rotate(m_transform.rotation * nu::DegToRad) * m_speed;
	m_life_timer.Decrement(dt);
	if (m_life_timer.IsElapsed()) m_destroyed = true;
	nu::PhysicsComponent* physicsComponent = GetComponent<nu::PhysicsComponent>();
	if (physicsComponent)
	{
		physicsComponent->SetVelocity(velocity);
		nu::Vector2 position = physicsComponent->GetPosition();
		position.x = nu::Clamp(0.0f, 1920.0f, position.x);
		position.y = nu::Clamp(0.0f, 1080.0f, position.y);
		physicsComponent->SetPosition(position);
	}
	Actor::Update(dt);
}

void Bullet::Read(const nu::json::value_t& value)
{
	Actor::Read(value);

	m_life_timer.SetTimer(m_lifespan);
	JSON_READ_NAME(value, "speed", m_speed);
}


