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

    m_ColorShader = Shader::Create("assets/shaders/Color.glsl");
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
        //m_MainCamera->UpdateCameraVectors();
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
      RenderCommand::SetClearColor({0.04f, 0.16f, 0.25f, 1.0f});
      RenderCommand::Clear();
      Renderer::BeginScene(*m_MainCamera);
      auto group = m_Registry.group<TagComponent>(entt::get<MeshComponent, TransformComponent, MaterialComponent>);

      // Render Opaque
      for (auto entity : group)
      {
        auto [mesh, trans, mat] = group.get<MeshComponent, TransformComponent, MaterialComponent>(entity);

        if (mat.IsOpaque == true)
        {
          mat._Texture->Bind();

          for (auto shap : mesh._Mesh->GetMesh())
          {
            Renderer::Submit(mat._Shader, shap.second, trans.GetTransform());
          }
        }
      }
      // Render Transpa
      for (auto entity : group)
      {
        auto [mesh, trans, mat] = group.get<MeshComponent, TransformComponent, MaterialComponent>(entity);

        if (mat.IsOpaque != true)
        {
          mat._Texture->Bind();

          for (auto shap : mesh._Mesh->GetMesh())
          {
            Renderer::Submit(mat._Shader, shap.second, trans.GetTransform());
          }
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

  void Scene::OnUpdateEditor(Camera& camera, Timestep timestep, float posX, float posY)
  {

    // Pick
    Renderer::BeginScene(camera);

    if (posX != 0.0f && posY != 0.0f)
    {
      DK_CORE_INFO("Pos:{0}, {1}", posX, posY);
      RenderCommand::SetClearColor({1.0f, 1.0f, 1.0f, 1.0f});
      RenderCommand::Clear();

      auto group = m_Registry.group<UUIDComponent>(entt::get<MeshComponent, TransformComponent>);
      for (auto entity : group)
      {
        auto [uuidComponent, mesh, trans] = group.get<UUIDComponent, MeshComponent, TransformComponent>(entity);

        size_t strHash = uuidComponent.Uuid.ConvertUUIDToHash();

        int r = (strHash & 0x000000FF) >> 0;
        int g = (strHash & 0x0000FF00) >> 8;
        int b = (strHash & 0x00FF0000) >> 16;

        std::dynamic_pointer_cast<OpenGLShader>(m_ColorShader)->UploadUniformFloat4("u_Color", {r / 255.0f, g / 255.0f, b / 255.0f, 1.0f});
        std::dynamic_pointer_cast<OpenGLShader>(m_ColorShader)->use();

        for (auto shap : mesh._Mesh->GetMesh())
        {
          Renderer::Submit(m_ColorShader, shap.second, trans.GetTransform());
        }
      }

      RenderCommand::FlushAndFinish();
      RenderCommand::PixelStore();

      unsigned char data[4];
      RenderCommand::ReadPixels(posX, posY, 1, 1, data);

      // Convert the color back to an integer ID
      int pickedID =
          data[0] +
          data[1] * 256 +
          data[2] * 256 * 256;

      if (pickedID == 0x00ffffff)
      { // Full white, must be the background !
        DK_CORE_TRACE("picked background");
      }
      else
      {
        auto view  = m_Registry.view<UUIDComponent, TagComponent>();

        for (auto entity : view)
        {
          auto& uuidComponent = view.get<UUIDComponent>(entity);
          size_t strHash      = uuidComponent.Uuid.ConvertUUIDToHash();

          int r = (strHash & 0x000000FF) >> 0;
          int g = (strHash & 0x0000FF00) >> 8;
          int b = (strHash & 0x00FF0000) >> 16;

          //DK_CORE_INFO("data:{0}, {1}, {2}, rgb:{3}, {4}, {5}", data[0], data[1], data[2], r, g, b);

          if (data[0] == r && data[1] == g && data[2] == b)
          {
            auto& tag = view.get<TagComponent>(entity);
            DK_CORE_INFO("Picked Entity:{0}", tag.Tag.c_str());
          }

        }

        DK_CORE_TRACE("picked: {0}", pickedID);
      }
    }

    RenderCommand::SetClearColor({0.0f, 0.0f, 0.0f, 1.0f});
    RenderCommand::Clear();

    auto group = m_Registry.group<TagComponent>(entt::get<MeshComponent, TransformComponent, MaterialComponent>);
    // Render Opaque
    for (auto entity : group)
    {
      auto [mesh, trans, mat] = group.get<MeshComponent, TransformComponent, MaterialComponent>(entity);

      if (mat.IsOpaque == true)
      {
        std::dynamic_pointer_cast<OpenGLShader>(mat._Shader)->UploadUniformInt("u_Texture", 0);
        std::dynamic_pointer_cast<OpenGLShader>(mat._Shader)->use();

        mat._Texture->Bind();
        for (auto shap : mesh._Mesh->GetMesh())
        {
          Renderer::Submit(mat._Shader, shap.second, trans.GetTransform());
        }
      }
    }
    // Render Trans
    for (auto entity : group)
    {
      auto [mesh, trans, mat] = group.get<MeshComponent, TransformComponent, MaterialComponent>(entity);

      if (mat.IsOpaque != true)
      {
        std::dynamic_pointer_cast<OpenGLShader>(mat._Shader)->UploadUniformInt("u_Texture", 0);
        std::dynamic_pointer_cast<OpenGLShader>(mat._Shader)->use();

        mat._Texture->Bind();
        for (auto shap : mesh._Mesh->GetMesh())
        {
          Renderer::Submit(mat._Shader, shap.second, trans.GetTransform());
        }
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