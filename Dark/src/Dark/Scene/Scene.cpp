#include "dkpch.h"

#include "Scene.h"
#include "Dark/Core/Utility/ECS/Entity.h"
#include "Dark/Core/Utility/ECS/Component.h"
#include "Platform/OpenGL/OpenGLShader.h"

#include "Dark/Renderer/Renderer.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include "glm/gtc/matrix_transform.hpp"

namespace Dark {

  Entity m_SelectEntity;

  Scene::Scene(const std::string& sceneName)
      : m_SceneName(sceneName)
  {
    Init();
  }

  Scene::~Scene()
  {
  }

  void Scene::DestroyEntity(Entity entity)
  {
    m_Registry.destroy(entity);
  }

  void Scene::OnUpdateRunTime(Timestep timestep)
  {
    // Update script
    {
      m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc) {
        // TODO : Move to ::ScenePlaye
        if (!nsc.Instance)
        {
          nsc.Instance = nsc.InstantiateScript();

          nsc.Instance->m_Entity = Entity{entity, this};
          nsc.Instance->OnCreate();
        }

        nsc.Instance->OnUpdate(timestep);
      });
    }

    /*
    {
      auto view = m_Registry.view<TransformComponent, MeshComponent, LightPropertyComponent>();

      int LightNums{0};

      for (auto entity : view)
      {
        auto [trans, mesh, lightProperty] = view.get<TransformComponent, MeshComponent, LightPropertyComponent>(entity);
        std::dynamic_pointer_cast<Shader>(m_shader)->UploadUniformFloat3("lights[" + std::to_string(LightNums) + "].position", trans.Translation);
        std::dynamic_pointer_cast<Shader>(m_shader)->UploadUniformFloat3("lights[" + std::to_string(LightNums) + "].direction", trans.Rotation);
        std::dynamic_pointer_cast<Shader>(m_shader)->UploadUniformFloat4("lights[" + std::to_string(LightNums) + "].color", lightProperty._Property->Color);
        std::dynamic_pointer_cast<Shader>(m_shader)->UploadUniformFloat("lights[" + std::to_string(LightNums) + "].intensity", lightProperty._Property->Intensity);
        std::dynamic_pointer_cast<Shader>(m_shader)->UploadUniformFloat("lights[" + std::to_string(LightNums) + "].range", lightProperty._Property->Range);
        std::dynamic_pointer_cast<Shader>(m_shader)->UploadUniformFloat("lights[" + std::to_string(LightNums) + "].spotAngle", glm::cos(glm::radians(lightProperty._Property->SpotAngle)));

        std::dynamic_pointer_cast<Shader>(m_shader)->UploadUniformInt("lights[" + std::to_string(LightNums) + "].type", lightProperty._Property->LightType);

        LightNums++;
      }

      std::dynamic_pointer_cast<Shader>(m_shader)->UploadUniformInt("LightNums", LightNums);
    }
    */

    {
      auto view = m_Registry.view<TagComponent, CameraComponent, TransformComponent>();
      for (auto entity : view)
      {
        auto [tag, camera, transform] = view.get<TagComponent, CameraComponent, TransformComponent>(entity);

        m_MainCamera = camera._Camera;
        m_MainCamera->SetPosition(transform.Translation);
        m_MainCamera->SetRotation(transform.Rotation);
        camera._Camera = m_MainCamera;
        //if (camera._Perspective)
        //{
        //  mainCamera = &camera._Camera;

        //  // TODO: recalculate camera entity transform
        //  //CameraTransform = &transform.Transform;
        //}
      }
    }

    // TODO: review mainCamera entity in scene
    if (m_MainCamera)
    {
      RenderCommand::SetClearColor({0.0f, 0.0f, 0.0f, 1.0f});
      RenderCommand::Clear();
      Renderer::BeginScene(*m_MainCamera);
      auto group = m_Registry.group<TagComponent>(entt::get<MeshComponent, TransformComponent, MaterialComponent>);

      for (auto entity : group)
      {
        auto [mesh, trans, mat] = group.get<MeshComponent, TransformComponent, MaterialComponent>(entity);

        mat._Texture->Bind();

        for (auto shap : mesh._Mesh->GetMesh())
        {
          Renderer::Submit(mat._Shader, shap.second, trans.GetTransform());
        }
      }
      Renderer::EndScene();
      //for (auto entity : group)
      //{
      //  // a component at a time ...
      //  //auto &position = group.get<position>(entity);
      //  //auto &velocity = group.get<velocity>(entity);

      //  // ... multiple components ...
      //  //auto[pos, vel] = group.get<position, velocity>(entity);

      //  // ... all components at once

      //  if (entity == m_SelectEntity.GetHandle())
      //    continue;

      //  auto [tag, mesh, trans] = group.get<TagComponent, MeshComponent, TransformComponent>(entity);

      //  for (auto shap : mesh._Mesh->GetMesh())
      //  {
      //    Renderer::Submit(m_shader, shap.second, trans.GetTransform());
      //  }
      //  // ...
      //}

      /*
      if (!m_SelectEntity.IsNullEntity() && m_SelectEntity.HasComponent<MeshComponent>())
      {
        Renderer::SetRenderState();

        auto mesh  = m_SelectEntity.GetComponent<MeshComponent>();
        auto trans = m_SelectEntity.GetComponent<TransformComponent>();

        for (auto shap : mesh._Mesh->GetMesh())
        {
          Renderer::Submit(m_shader, shap.second, trans.GetTransform());
        }
      }
      */

      /*
      {
        auto view = m_Registry.view<TransformComponent, MeshComponent, LightPropertyComponent>();
        int LightNums{0};
        for (auto entity : view)
        {
          auto [trans, mesh, lightProperty] = view.get<TransformComponent, MeshComponent, LightPropertyComponent>(entity);
          std::dynamic_pointer_cast<Shader>(m_ColorShader)->use();
          std::dynamic_pointer_cast<Shader>(m_ColorShader)->UploadUniformFloat4("u_Color", lightProperty._Property->Color);

          for (auto shap : mesh._Mesh->GetMesh())
          {
            Renderer::Submit(m_ColorShader, shap.second, trans.GetTransform());
          }

          LightNums++;
        }
      }
     */
    }
  }

  void Scene::OnUpdateEditor(Camera& camera, Timestep timestep)
  {
    RenderCommand::SetClearColor({0.0f, 0.0f, 0.0f, 1.0f});
    RenderCommand::Clear();

    Renderer::BeginScene(camera);
    auto group = m_Registry.group<TagComponent>(entt::get<MeshComponent, TransformComponent, MaterialComponent>);

    for (auto entity : group)
    {
      auto [mesh, trans, mat] = group.get<MeshComponent, TransformComponent, MaterialComponent>(entity);

      std::dynamic_pointer_cast<OpenGLShader>(mat._Shader)->UploadUniformInt("u_Texture", 0);
      std::dynamic_pointer_cast<OpenGLShader>(mat._Shader)->use();

      mat._Texture->Bind();

      for (auto shap : mesh._Mesh->GetMesh())
      {
        Renderer::Submit(mat._Shader, shap.second, trans.GetTransform());
      }
    }
    Renderer::EndScene();
  }

  void Scene::Init()
  {
    RenderCommand::SetClearColor({0.0f, 0.0f, 0.0f, 1.0f});
    RenderCommand::Clear();
  }

  //void Scene::SetSelectEntity(Entity entity)
  //{
  //  m_SelectEntity = entity;
  //}

} // namespace Dark