#pragma once

#include "Framework/Actor.h"
#include "Timer.h"

struct BulletDesc : public nu::ActorDesc
{
	float speed = 0.0f;
};

class Bullet : public nu::Actor
{
public:
	Bullet() = default;
	Bullet(const BulletDesc& BulletDesc) :
		Actor{ BulletDesc },
		m_speed{ BulletDesc.speed }
	{}

	CLASS_PROTOTYPE(Bullet)

	void Update(float dt) override;

	void SetSpeed(float speed) { m_speed = speed; }

	virtual void Read(const nu::json::value_t& value) override;

private:
	float m_speed = 175.0f;
	nu::Timer m_life_timer{ 1.0f };
};