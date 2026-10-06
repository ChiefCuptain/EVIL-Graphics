#include "pch.h"
#include "Physics.h"
#include "Framework/Actor.h"
namespace nu
{
	float Physics::m_pixelsPerUnit = 48.0f;

	bool Physics::Initialize()
	{
		b2WorldDef worldDef = b2DefaultWorldDef();
		worldDef.gravity = b2Vec2{ 0.0f, 10.0f };
		worldDef.maximumLinearSpeed = 4000.0f;
		m_worldId = b2CreateWorld(&worldDef);
		b2World_EnableSleeping(m_worldId, false);
		return true;
	}

	void Physics::Quit()
	{
		b2DestroyWorld(m_worldId);
	}

	void Physics::Update(float dt)
	{
		b2World_Step(m_worldId, 1.0f / 60.0f, 4);
		ProcessPhysicsEvents();
	}

	void Physics::ProcessPhysicsEvents()
	{
		b2ContactEvents contact_events = b2World_GetContactEvents(m_worldId);
		
		for (int i = 0; i < contact_events.beginCount; ++i)
		{
			auto contact_event = contact_events.beginEvents + i;

			if (!b2Shape_IsValid(contact_event->shapeIdA) || !b2Shape_IsValid(contact_event->shapeIdB)) continue;

			b2BodyId bodyA = b2Shape_GetBody(contact_event->shapeIdA);
			b2BodyId bodyB = b2Shape_GetBody(contact_event->shapeIdB);

			Actor* actorA = (Actor*)b2Body_GetUserData(bodyA);
			if (!actorA || actorA->GetDestroyed() || !actorA->IsActive()) continue;
			Actor* actorB = (Actor*)b2Body_GetUserData(bodyB);
			if (!actorB || actorB->GetDestroyed() || !actorB->IsActive()) continue;

			actorA->OnCollision(actorB);
			if (!actorB || actorB->GetDestroyed() || !actorB->IsActive()) continue;
			actorB->OnCollision(actorA);
		}

		//Get Sensor Events
		b2SensorEvents sensor_events = b2World_GetSensorEvents(m_worldId);

		for (int i = 0; i < sensor_events.beginCount; ++i)
		{
			auto sensor_event = sensor_events.beginEvents + i;

			if (!b2Shape_IsValid(sensor_event->sensorShapeId) || !b2Shape_IsValid(sensor_event->visitorShapeId)) continue;

			b2BodyId bodyA = b2Shape_GetBody(sensor_event->sensorShapeId);
			b2BodyId bodyB = b2Shape_GetBody(sensor_event->visitorShapeId);

			Actor* actorA = (Actor*)b2Body_GetUserData(bodyA);
			if (!actorA || actorA->GetDestroyed() || !actorA->IsActive()) continue;
			Actor* actorB = (Actor*)b2Body_GetUserData(bodyB);
			if (!actorB || actorB->GetDestroyed() || !actorB->IsActive()) continue;

			actorA->OnCollision(actorB);
			if (!actorB || actorB->GetDestroyed() || !actorB->IsActive()) continue;
			actorB->OnCollision(actorA);
		}
	}
}
