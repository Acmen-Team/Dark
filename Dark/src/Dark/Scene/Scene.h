#pragma once
#include "Dark/Core/Timestep.h"

#include "Dark/Resource/Shader.h"
#include "Dark/Renderer/Camera.h"

#include <entt.hpp>

namespace Dark {

  class Entity;

  class Scene
  {
  public:
    Scene(const std::string& sceneName);
    ~Scene();

    template <typename EntityT>
    Entity CreatEntity(const std::string& tagStr = std::string())
    {
      Entity entity = {m_Registry.create(), this};

      auto& tag      = entity.AddComponent<TagComponent>();
      tag.Tag        = tagStr.empty() ? "Entity" : tagStr;

      entity.AddComponent<UUIDComponent>();
      entity.AddComponent<TransformComponent>();

      return entity;
    }

    void DestroyEntity(Entity entity);
    // Temp
    void OnUpdateRunTime(Timestep timestep, float posX, float posY);

    void OnUpdateEditor(Camera& camera, Timestep timestep, float posX, float posY);

    std::string& GetSceneName() { return m_SceneName; }

    void SetSelectEntity(Ref<Entity> entity) { m_SelectEntity = entity; }
    Ref<Entity> GetSelectEntity() { return m_SelectEntity; }
  private:
    void Init();
  private:
    std::string m_SceneName;

    entt::registry m_Registry;

    friend class Entity;
    friend class SceneHierarchyPanel;
    friend class Serialize;

    //Epoch::ShaderLibrary* m_ShaderLibrary;
    //std::shared_ptr<Shader> m_ColorShader;
    Ref<Shader> m_ColorShader;
    Ref<Entity> m_SelectEntity{nullptr};

    Ref<Camera> m_MainCamera = nullptr;

    glm::mat4* CameraTransform = nullptr;
  };

} // namespace Epoch