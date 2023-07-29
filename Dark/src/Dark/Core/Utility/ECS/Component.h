/*************************************************
Copyright © 2020 - 2022 YX.All Rights Reserved

Date:2022-10-7 19:24
Description:Component
**************************************************/
#pragma once
#include "Dark/Scene/ScriptableEntity.h"
#include "Dark/Resource/Mesh.h"
#include "Dark/Resource/Texture.h"
#include "Dark/Scene/Light.h"

#include <glm/glm.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include <string>

namespace Dark {

  struct UUIDComponent
  {
    UUID Uuid{};

    UUIDComponent()
    {
    }
    UUIDComponent(const UUIDComponent&) = default;
  };

  struct TagComponent
  {
    std::string Tag;

    TagComponent()
    {
    }
    TagComponent(const TagComponent&) = default;
    TagComponent(const std::string& tag)
        : Tag(tag) {}
  };

  struct TransformComponent
  {
    glm::vec3 Translation = {0.0f, 0.0f, 0.0f};
    glm::vec3 Rotation    = {0.0f, 0.0f, 0.0f};
    glm::vec3 Scale       = {1.0f, 1.0f, 1.0f};

    TransformComponent()                          = default;
    TransformComponent(const TransformComponent&) = default;
    TransformComponent(const glm::vec3& translation, const glm::vec3& rotation, const glm::vec3& scale)
        : Translation(translation), Rotation(rotation), Scale(scale)
    {
    }

    glm::mat4 GetTransform() const
    {
      // calculate transform matrix
      glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

      glm::mat4 transformation = glm::translate(glm::mat4(1.0f), Translation) * rotation * glm::scale(glm::mat4(1.0f), Scale);

      return transformation;
    }
  };

  struct MeshComponent
  {
    Ref<Mesh> _Mesh;

    MeshComponent()                     = default;
    MeshComponent(const MeshComponent&) = default;
    MeshComponent(const Ref<Mesh>& mesh)
        : _Mesh(mesh)
    {
    }
  };

  struct MaterialComponent
  {
    Ref<Shader> _Shader;
    Ref<Texture> _Texture;
    bool IsOpaque;
    MaterialComponent()                         = default;
    MaterialComponent(const MaterialComponent&) = default;
    MaterialComponent(const Ref<Shader>& shader, const Ref<Texture>& texture)
        : _Shader(shader), _Texture(texture)
    {
    }
  };

  struct NativeScriptComponent
  {
    ScriptableEntity* Instance = nullptr;

    // Using function ptr replace std::function
    ScriptableEntity* (*InstantiateScript)();
    void (*DestroyScript)(NativeScriptComponent*);

    template <typename T>
    void Bind()
    {
      InstantiateScript = []() {
        return static_cast<ScriptableEntity*>(new T());
      };
      DestroyScript = [](NativeScriptComponent* nsc) {
        delete nsc->Instance;
        nsc->Instance = nullptr;
      };
    }
  };

  struct CameraComponent
  {
    Ref<Camera> _Camera;
    CameraComponent()                       = default;
    CameraComponent(const CameraComponent&) = default;
    CameraComponent(const Ref<Camera>& camera)
        : _Camera(camera)
    {
    }
  };

  struct LightComponent
  {
    Ref<LightProperty> _LightProperty;
    LightComponent()                      = default;
    LightComponent(const LightComponent&) = default;
    LightComponent(const Ref<LightProperty>& lightProp)
        : _LightProperty(lightProp)
    {
    }
  };

} // namespace Dark