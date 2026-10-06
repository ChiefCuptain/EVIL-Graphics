#pragma once

#pragma once

#include "Framework/Actor.h"
#include "Components/PhysicsComponent.h"

struct EnemyDesc : public nu::ActorDesc
{
	float speed = 175.0f;
	float fire_cooldown = 2.0f;
};

class Enemy : public nu::Actor
{
public:
	Enemy() = default;
	Enemy(const EnemyDesc& enemyDesc) :
		Actor{ enemyDesc },
		m_speed{ enemyDesc.speed },
		m_fire_cooldown{ enemyDesc.fire_cooldown }
	{ }

	CLASS_PROTOTYPE(Enemy)


	void Update(float dt) override;

	void OnCollision(Actor* other) override;

	virtual void Read(const nu::json::value_t& value) override;

private:
	float m_fire_cooldown = 2.0f;
	float m_fire_timer = 0.0f;
	float m_speed = 175.0f;
	float m_brake_speed = 4.0f;
};