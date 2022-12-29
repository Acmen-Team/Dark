#include "dkpch.h"

#include "Serialize.h"

#include <yaml-cpp/yaml.h>

#include "Dark/Core/Utility/ECS/Entity.h"
#include "Dark/Core/Utility/ECS/Component.h"
#include "Dark/Resource/ResourceManager.h"

namespace YAML {

  template <>
  struct convert<glm::vec3>
  {
    static Node encode(const glm::vec3& v)
    {
      Node node;
      node.push_back(v.x);
      node.push_back(v.y);
      node.push_back(v.z);
      return node;
    }

    static bool decode(const Node& node, glm::vec3& v)
    {
      if (!node.IsSequence() || node.size() != 3)
        return false;

      v.x = node[0].as<float>();
      v.y = node[1].as<float>();
      v.z = node[2].as<float>();
      return true;
    }
  };

  template <>
  struct convert<glm::vec4>
  {
    static Node encode(const glm::vec4& v)
    {
      Node node;
      node.push_back(v.x);
      node.push_back(v.y);
      node.push_back(v.z);
      node.push_back(v.w);
      return node;
    }

    static bool decode(const Node& node, glm::vec4& v)
    {
      if (!node.IsSequence() || node.size() != 4)
        return false;

      v.x = node[0].as<float>();
      v.y = node[1].as<float>();
      v.z = node[2].as<float>();
      v.w = node[3].as<float>();
      return true;
    }
  };

} // namespace YAML

namespace Dark {

  YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
  {
    out << YAML::Flow;
    out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;

    return out;
  }

  YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
  {
    out << YAML::Flow;
    out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;

    return out;
  }

  Serialize::Serialize()
  {
    m_ShaderLibrary.Load("assets/shaders/Texture.glsl");
  }

  Serialize::Serialize(const Ref<Scene>& scene)
      : m_Scene(scene)
  {
    m_ShaderLibrary.Load("assets/shaders/Texture.glsl");
  }

  static void SerializeEntity(YAML::Emitter& out, Entity entity)
  {
    out << YAML::BeginMap;
    out << YAML::Key << "Entity" << YAML::Value << entity.GetUUID().ConvertUUIDToString();

    if (entity.HasComponent<TagComponent>())
    {
      out << YAML::Key << "TagComponent";

      out << YAML::BeginMap;

      auto& tag = entity.GetComponent<TagComponent>().Tag;

      out << YAML::Key << "Tag" << YAML::Value << tag;

      out << YAML::EndMap;
    }

    if (entity.HasComponent<TransformComponent>())
    {
      out << YAML::Key << "TransformComponent";
      out << YAML::BeginMap;

      auto& trans = entity.GetComponent<TransformComponent>();

      out << YAML::Key << "Translation" << YAML::Value << trans.Translation;
      out << YAML::Key << "Rotation" << YAML::Value << trans.Rotation;
      out << YAML::Key << "Scale" << YAML::Value << trans.Scale;

      out << YAML::EndMap;
    }

    if (entity.HasComponent<CameraComponent>())
    {
      out << YAML::Key << "CameraComponent";
      out << YAML::BeginMap;

      auto& camera = entity.GetComponent<CameraComponent>();

      out << YAML::Key << "Camera";
      out << YAML::BeginMap;
      out << YAML::Key << "ProjectType" << YAML::Value << (int)camera._Camera->GetProjectionType();
      out << YAML::Key << "PerspectiveFov" << YAML::Value << camera._Camera->GetPerspectiveFov();
      out << YAML::Key << "PerspectiveNear" << YAML::Value << camera._Camera->GetPerspectiveNear();
      out << YAML::Key << "PerspectiveFar" << YAML::Value << camera._Camera->GetPerspectiveFar();

      out << YAML::Key << "OrthographicSize" << YAML::Value << camera._Camera->GetOrthographicSize();
      out << YAML::Key << "OrthographicNear" << YAML::Value << camera._Camera->GetOrthographicNear();
      out << YAML::Key << "OrthographicFar" << YAML::Value << camera._Camera->GetOrthographicFar();
      out << YAML::EndMap;

      out << YAML::Key << "FixedAspectration" << YAML::Value << camera._Camera->fixedAspectration;

      out << YAML::EndMap;
    }

    if (entity.HasComponent<MeshComponent>())
    {
      out << YAML::Key << "MeshComponent";
      out << YAML::BeginMap;

      auto& mesh = entity.GetComponent<MeshComponent>();

      out << YAML::Key << "MeshName" << YAML::Value << mesh._Mesh->GetMeshName();
      //out << YAML::Key << "UUID" << YAML::Value << mesh._Mesh->GetResourceID()->uuid->GetUUIDString();

      out << YAML::EndMap;
    }

    if (entity.HasComponent<MaterialComponent>())
    {
      out << YAML::Key << "MaterialComponent";
      out << YAML::BeginMap;

      auto& material = entity.GetComponent<MaterialComponent>();

      out << YAML::Key << "Shader";
      out << YAML::BeginMap;
      out << YAML::Key << "ShaderName" << YAML::Value << material._Shader->GetName();
      out << YAML::EndMap;

      out << YAML::Key << "Texture";
      out << YAML::BeginMap;
      out << YAML::Key << "TexturePath" << YAML::Value << ResourceManager::Get().GetResourceAllocator()->GetResourceID(material._Texture)->path;
      out << YAML::Key << "TextureUUID" << YAML::Value << ResourceManager::Get().GetResourceAllocator()->GetResourceID(material._Texture)->uuid->ConvertUUIDToString();
      out << YAML::EndMap;

      out << YAML::EndMap;
    }

    out << YAML::EndMap;
  }

  void Serialize::SerializeRuntime(const std::string& filePath)
  {
    YAML::Emitter out;
    out << YAML::BeginMap;
    out << YAML::Key << "Scene";
    out << YAML::Value << m_Scene->GetSceneName();
    out << YAML::Key << "Entities";
    out << YAML::Value << YAML::BeginSeq;
    m_Scene->m_Registry.each([&](auto entityID) {
      Entity entity = {entityID, m_Scene.get()};

      if (!entity)
        return;

      SerializeEntity(out, entity);
    });
    out << YAML::EndSeq;
    out << YAML::EndMap;

    std::ofstream fout(filePath);
    fout << out.c_str();

    fout.close();
  }

  Ref<Scene> Serialize::DeserializeRuntime(const std::string& filePath)
  {
    std::ifstream stream(filePath);
    std::stringstream strStream;
    strStream << stream.rdbuf();

    YAML::Node data = YAML::Load(strStream.str());
    if (!data["Scene"])
      return nullptr;

    std::string sceneName = data["Scene"].as<std::string>();
    DK_CORE_TRACE("Deserializing scene: {0}", sceneName);

    Ref<Scene> deserializedScene = CreateRef<Scene>(sceneName);

    auto entities = data["Entities"];
    if (entities)
    {
      for (auto entity : entities)
      {
        std::string uuid = entity["Entity"].as<std::string>();

        std::string name;
        auto tagComponent = entity["TagComponent"];
        if (tagComponent)
          name = tagComponent["Tag"].as<std::string>();

        DK_CORE_TRACE("Deserialized entity with UUID = {0}, name = {1}", uuid, name);

        Entity deserializedEntity = deserializedScene->CreatEntity<Entity>(name);

        auto transformComponent = entity["TransformComponent"];
        if (transformComponent)
        {
          auto& trans = deserializedEntity.GetComponent<TransformComponent>();

          trans.Translation = transformComponent["Translation"].as<glm::vec3>();
          trans.Rotation    = transformComponent["Rotation"].as<glm::vec3>();
          trans.Scale       = transformComponent["Scale"].as<glm::vec3>();
        }

        auto cameraComponent = entity["CameraComponent"];
        if (cameraComponent)
        {
          auto& camera   = deserializedEntity.AddComponent<CameraComponent>();
          camera._Camera = CreateRef<Camera>();

          auto& cameraProps = cameraComponent["Camera"];
          camera._Camera->SetProjectionType((ProjectionType)cameraProps["ProjectType"].as<int>());
          camera._Camera->SetPerspectiveFov(cameraProps["PerspectiveFov"].as<float>());
          camera._Camera->SetPerspectiveNear(cameraProps["PerspectiveNear"].as<float>());
          camera._Camera->SetPerspectiveFar(cameraProps["PerspectiveFar"].as<float>());
          camera._Camera->SetOrthographicSize(cameraProps["OrthographicSize"].as<float>());
          camera._Camera->SetOrthographicNear(cameraProps["OrthographicNear"].as<float>());
          camera._Camera->SetOrthographicFar(cameraProps["OrthographicFar"].as<float>());

          camera._Camera->fixedAspectration = cameraComponent["FixedAspectration"].as<bool>();
        }

        auto meshComponent = entity["MeshComponent"];
        if (meshComponent)
        {
          auto& mesh = deserializedEntity.AddComponent<MeshComponent>();

          std::string meshName = meshComponent["MeshName"].as<std::string>();
          if (meshName == "Panel")
            mesh._Mesh = ResourceManager::Get().s_PanelMesh;
          // TODO: Get mesh resource from ResourceManager
        }

        auto materialComponent = entity["MaterialComponent"];
        if (materialComponent)
        {
          auto& material = deserializedEntity.AddComponent<MaterialComponent>();

          auto& shaders  = materialComponent["Shader"];
          auto& textures = materialComponent["Texture"];

          std::string shaderName  = shaders["ShaderName"].as<std::string>();
          std::string texturePath = textures["TexturePath"].as<std::string>();

          // TODO: Get Shader resource from RM
          material._Shader = m_ShaderLibrary.Get(shaderName);
          // TODO: Get Texture resource from ResourceManager
          material._Texture = ResourceManager::Get().GetResourceAllocator()->GetResource<Texture>(texturePath);
        }
      }
    }

    stream.close();

    return deserializedScene;
  }

} // namespace Dark