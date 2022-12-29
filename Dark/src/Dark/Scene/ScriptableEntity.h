#pragma once

#include "Dark/Core/Timestep.h"

#include "Dark/Core/Utility/ECS/Entity.h"

namespace Dark {

  class ScriptableEntity
  {
  public:
    virtual ~ScriptableEntity() = default;

    template <typename T>
    T& GetComponent()
    {
      return m_Entity.GetComponent<T>();
    }

  protected:
    virtual void OnCreate() {}
    virtual void OnUpdate(Timestep timestep) {}
    virtual void OnDestroy() {}

  private:
    Entity m_Entity;
    friend class Scene;
  };

} // namespace Epoch
