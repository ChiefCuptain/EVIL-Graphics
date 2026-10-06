#include "pch.h"
#include "Framework/Actor.h"
#include "Renderer/Renderer.h"
#include "Math/MathUtil.h"
#include "Renderer/Texture.h"
#include "Core/Factory.h"
#include "Resources/ResourceManager.h"
#include "Components/RendererComponent.h"


namespace nu {

    FACTORY_REGISTER(Actor);

    Actor::Actor(const Actor& other) :
        Object{other},
        m_tag{other.m_tag},
        m_transform{other.m_transform}
    {
        for (const auto& component : other.m_components)
        {
            auto clone = std::unique_ptr<Component>(dynamic_cast<Component*>(component->Clone().release()));
            AddComponent(std::move(clone));
        }
    }

    void Actor::Start()
    {
        for (auto& component : m_components)
        {
            component->Start();
        }
    }

    void Actor::Update(float dt)
    {

        ProcessLifespan(dt);

        for (auto& component : m_components)
        {
            component->Update(dt);
        }

    }

    void nu::Actor::Draw(const Renderer& r) const
    {
        for (auto& component : m_components)
        {
            auto rendererComponent = dynamic_cast<RendererComponent*>(component.get());
            if (rendererComponent)
            {
                rendererComponent->Draw(r);
            }
        }

    }

    void Actor::OnDestroy()
    {
        for (auto& component : m_components)
        {
            component->Destroyed();
        }
    }

    void Actor::SetTransform(const Transform& transform)
    {
        SetPosition(transform.position);
        SetRotation(transform.rotation);
        SetScale(transform.scale);
    }

    void Actor::ProcessLifespan(float dt)
    {
        if (m_lifespan != -1.0f)
        {
            m_lifespan -= dt;
            m_destroyed = (m_lifespan <= 0.0f);
        }
    }

    void Actor::Read(const json::value_t& value)
    {
        Object::Read(value);

        JSON_READ_NAME(value, "tag", m_tag);
        JSON_READ_NAME(value, "type", m_type);
        if (JSON_HAS_NAME(value, "transform")) m_transform.Read(JSON_GET_NAME(value, "transform"));
        auto& test = JSON_GET_NAME(value, "transform");
        JSON_READ_NAME(value, "persistent", m_persistent);
        JSON_READ_NAME(value, "lifespan", m_lifespan);

        if (JSON_HAS_NAME(value, "components"))
        {
            for (auto& componentValue : JSON_GET_NAME(value, "components").GetArray())
            {
                std::string typeName;
                JSON_READ_NAME(componentValue, "type", typeName);

                std::cout << "Loading component type : " << typeName << "\n";

                auto component = Factory::Instance().Create<Component>(typeName);

                if (component)
                {
                    component->Read(componentValue);
                    AddComponent(std::move(component));
                }
            }
        }
    }

    void Actor::AddComponent(std::unique_ptr<Component> component)
    {
        component->SetOwner(this);
        m_components.push_back(std::move(component));
    }
}