#include "pch.h"
#include "Enemy.h"
#include "Player.h"
#include "Renderer/Renderer.h"
#include "Math/MathUtil.h"
#include "Engine.h"
#include "Bullet.h"
#include "Assets.h"
#include "Resources/ResourceManager.h"

#include <cmath>

FACTORY_REGISTER(Enemy);

void Enemy::Update(float dt)
{
    m_fire_timer += dt;

    Player* player = m_scene->GetActorByName<Player>("PlayerPrototype");
    if (player)
    {
        nu::Vector2 direction = player->GetTransform().position - m_transform.position;
        float rotation = direction.AngleTo(nu::Vector2{0.0f});
        nu::PhysicsComponent* physicsComponent = GetComponent<nu::PhysicsComponent>();
        if (physicsComponent)
        {
            nu::Vector2 forward{ 1, 0 };
            forward = forward.Rotate(m_transform.rotation * nu::DegToRad);
            nu::Vector2 force = forward * m_speed;

            physicsComponent->ApplyForce(force);
            physicsComponent->SetRotation(rotation * nu::DegToRad);


            if ((physicsComponent->GetVelocity().x > 0 && GetTransform().position.x > player->GetTransform().position.x) || (physicsComponent->GetVelocity().x < 0 && GetTransform().position.x < player->GetTransform().position.x))
            {
                physicsComponent->SetVelocity({ physicsComponent->GetVelocity().x * (1.0f / (1.0f + m_brake_speed * dt)), physicsComponent->GetVelocity().y });
            }
            if ((physicsComponent->GetVelocity().y > 0 && GetTransform().position.y > player->GetTransform().position.y) || (physicsComponent->GetVelocity().y < 0 && GetTransform().position.y < player->GetTransform().position.y))
            {
                physicsComponent->SetVelocity({ physicsComponent->GetVelocity().x, physicsComponent->GetVelocity().y * (1.0f / (1.0f + m_brake_speed * dt)) });
            }

            nu::Vector2 position = physicsComponent->GetPosition();
            position.x = nu::Clamp(0.0f, 1920.0f, position.x);
            position.y = nu::Clamp(0.0f, 1080.0f, position.y);
            physicsComponent->SetPosition(position);

        }

        if (m_fire_timer > m_fire_cooldown)
        {
            m_fire_timer = 0.0f;

            auto actor = nu::Factory::Instance().Create<Bullet>("BulletPrototype");

            actor->SetTag("Enemy_Bullet");
            actor->SetTransform(m_transform);
            actor->SetScale(1.0f);
  
            m_scene->AddActor(std::move(actor));

            nu::Engine::Get().GetAudio().PlaySound("enemy_shoot");

        }

    } 



    Actor::Update(dt);
}

void Enemy::OnCollision(Actor* other)
{
    if (other->GetTag() == "Player_Bullet")
    {
        for (int i = 0; i < 100; i++)
        {
            nu::Particle particle;
            particle.position = m_transform.position;
            particle.color = { 1.0f, 1.0f, 1.0f };
            particle.lifespan = nu::RandomFloat(0.5f, 2.0f);
            particle.velocity = { nu::RandomFloat(-600.0f, 600.0f), nu::RandomFloat(-600.0f, 600.0f) };

            nu::Engine::Get().GetPS().AddParticle(particle, nu::Resources().GetWithID<nu::Texture>("explosion", "Textures/spr_explosion.png", nu::Engine::Get().GetRenderer()));

        }
        nu::Engine::Get().GetAudio().PlaySound("explosion");
        SetDestroyed();
        other->SetDestroyed();
        
    }
}

void Enemy::Read(const nu::json::value_t& value)
{
    Actor::Read(value);

    JSON_READ_NAME(value, "fire_cooldown", m_fire_cooldown);
    JSON_READ_NAME(value, "speed", m_speed);
    JSON_READ_NAME(value, "brake_speed", m_brake_speed);
}

