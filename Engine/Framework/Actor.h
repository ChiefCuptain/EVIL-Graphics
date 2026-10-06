#pragma once
#include "Framework/Object.h"
#include "Math/Transform.h"
#include "Renderer/Model.h"
#include "Resources/Resource.h"
#include "Components/Component.h"
#include <string>
namespace nu
{
    class Scene;
    class Texture;

    struct ActorDesc
    {
        std::string name = "undefined";
        std::string tag = "undefined";
        Transform transform;
        Vector2 velocity{ 0.0f };
        float lifespan = -1.0f;
        res_t<Model> model;
        res_t<Texture> texture;
    };

    class Actor : public Object
    {
    public:
        Actor() = default;
        Actor(const ActorDesc& actorDesc) :
            Object{ actorDesc.name },
            m_tag{ actorDesc.tag },
            m_transform{ actorDesc.transform },
            m_lifespan{ actorDesc.lifespan }
        { }

        Actor(const Actor& other);

        CLASS_PROTOTYPE(Actor)

        virtual void Start();
        virtual void Update(float dt);
        virtual void Draw(const class Renderer& r) const;
        virtual void OnDestroy();

        virtual void OnCollision(Actor* other) {}

        const Transform& GetTransform() const { return m_transform; }

        void SetTransform(const Transform& transform);

        void SetPosition(const Vector2& position) { m_transform.position = position; }

        void SetRotation(float rotation) { m_transform.rotation = rotation; }

        void SetScale(float scale) { m_transform.scale = scale; }

        const std::string& GetName() const { return m_name; }
        const std::string& GetTag() const { return m_tag; }
        void SetTag(const std::string& tag) { m_tag = tag; }
        const Scene* GetScene() { return m_scene; }

        void SetDestroyed(bool destroy = true) { m_destroyed = destroy; }
        bool GetDestroyed() const { return m_destroyed; }

        void SetDestroyedQueued(bool destroy = true) { m_queued_destruction = destroy; }
        bool GetDestroyedQueued() const { return m_queued_destruction; }

        void ProcessLifespan(float dt);

        float GetRadius() const;

        bool GetPersistent() const { return m_persistent; }

        virtual void Read(const json::value_t& value) override;

        void AddComponent(std::unique_ptr<Component> component);

        template <std::derived_from<Component> T>
        T* GetComponent();

        friend Scene;

    protected:
        std::string m_tag;
        std::string m_type;
        Transform m_transform;
        float m_lifespan = -1.0f;
        bool m_destroyed = false;
        bool m_queued_destruction = false;
        bool m_persistent = false;

        std::vector<std::unique_ptr<Component>> m_components;

        Scene* m_scene = nullptr;
    };

    template<std::derived_from<Component> T>
    inline T* Actor::GetComponent()
    {
        for (auto& component : m_components)
        {
            auto result = dynamic_cast<T*>(component.get());
            if (result)
            {
                return result;
            }
        }

        return nullptr;
    }
}