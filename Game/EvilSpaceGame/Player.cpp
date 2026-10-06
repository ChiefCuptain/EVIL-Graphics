#include "pch.h"
#include "Player.h"
#include "Renderer/Renderer.h"
#include "Math/MathUtil.h"
#include "Engine.h"
#include "Assets.h"
#include "Bullet.h"
#include "Resources/ResourceManager.h"
#include "Components/PhysicsComponent.h"
#include <Components/RigidBodyPhysicsComponent.h>

FACTORY_REGISTER(Player);


void Player::Update(float dt)
{
    
        //nu::Vector2 force{ 0, 0 };
        float thrust = 0.0f;

        if (nu::Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_W)) thrust = m_speed;
        if (nu::Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_S)) thrust = -m_speed;

        float rotate = 0.0f;

        if (nu::Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_A)) rotate = -120.0f;
        if (nu::Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_D)) rotate = 120.0f;

        if (nu::Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_LSHIFT))
        {
            thrust *= 2.0f;
            rotate *= 2.0f;
        }
        else if (nu::Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_LCTRL))
        {
            thrust *= 0.5f;
            rotate *= 0.5f;
        }

        nu::PhysicsComponent* physicsComponent = GetComponent<nu::PhysicsComponent>();
        if (physicsComponent)
        {
            nu::Vector2 forward{ 1, 0 };
            nu::Vector2 force = forward.Rotate(m_transform.rotation * nu::DegToRad) * thrust;

            physicsComponent->ApplyForce(force);
            physicsComponent->ApplyTorque(rotate * nu::DegToRad);

            if (nu::Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_SPACE)) physicsComponent->SetVelocity(physicsComponent->GetVelocity() * (1.0f / (1.0f + m_brake_speed * dt)));

            nu::Vector2 position = physicsComponent->GetPosition();
            position.x = nu::Clamp(0.0f, 1920.0f, position.x);
            position.y = nu::Clamp(0.0f, 1080.0f, position.y);
            physicsComponent->SetPosition(position);

        }

        // Fire

        if (nu::Engine::Get().GetInput().GetKeyPressed(SDL_SCANCODE_E))
        {
            auto actor = nu::Factory::Instance().Create<Bullet>("BulletPrototype");

            actor->SetTag("Player_Bullet");
            actor->SetTransform(m_transform);
            actor->SetScale(1.0f);
            actor->SetSpeed(800.0f);

            m_scene->AddActor(std::move(actor));

            nu::Engine::Get().GetAudio().PlaySound("player_shoot");
        }
    
    Actor::Update(dt);
}

void Player::OnCollision(Actor* other)
{
    if (other->GetTag() == "Enemy_Bullet")
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

void Player::Read(const nu::json::value_t& value)
{
    Actor::Read(value);

    JSON_READ_NAME(value, "speed", m_speed);
    JSON_READ_NAME(value, "brake_speed", m_brake_speed);
}
