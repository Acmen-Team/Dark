#pragma once

#include "Dark/Core/UUID.h"
#include "Dark/Scene/Scene.h"

namespace Dark {

  class Entity
  {
  public:
    Entity()                    = default;
    Entity(const Entity& other) = default;
    Entity(entt::entity handle, Scene* scene)
        : m_EntityHandle(handle), m_Scene(scene)
    {
    }
    virtual ~Entity() = default;

    bool IsNullEntity() const
    {
      return m_EntityHandle == entt::null;
    }

    template <typename T>
    bool HasComponent()
    {
      return m_Scene->m_Registry.all_of<T>(m_EntityHandle);
    }

    template <typename T, typename... Args>
    T& AddComponent(Args&&... args)
    {
      DK_CORE_ASSERT(!HasComponent<T>(), "Entity already has component!");
      return m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
    }

    template <typename T>
    void RemoveComponent()
    {
      DK_CORE_ASSERT(HasComponent<T>(), "Entity doesn't have component!");
      m_Scene->m_Registry.remove<T>(m_EntityHandle);
    }

    template <typename T>
    T& GetComponent()
    {
      DK_CORE_ASSERT(HasComponent<T>(), "Entity doesn't have component!");
      return m_Scene->m_Registry.get<T>(m_EntityHandle);
    }

    operator bool() const
    {
      if (m_Scene == nullptr)
        return false;
      return m_Scene->m_Registry.valid(m_EntityHandle);
    }

    operator uint32_t() const
    {
      return (uint32_t)m_EntityHandle;
    }

    operator entt::entity() const { return m_EntityHandle; }

    bool operator==(const Entity& other) const
    {
      return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene;
    }

    bool operator!=(const Entity& other) const
    {
      return !(*this == other);
    }

    entt::entity GetHandle() const
    {
      return m_EntityHandle;
    }

  private:
    entt::entity m_EntityHandle{entt::null};
    Scene* m_Scene = nullptr;
  };

} // namespace Epoch