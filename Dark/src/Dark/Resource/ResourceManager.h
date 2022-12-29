/*************************************************
Copyright © 2020 - 2022 YX.All Rights Reserved

Date:2022-8-14 15:17
Description:ResourceManager
**************************************************/
#pragma once

#include "Dark/Resource/Mesh.h"
#include "Dark/Resource/Texture.h"
#include "Dark/Core/IO/FileSystem.h"
#include "Dark/Resource/Shader.h"

#include "Dark/Tools/Profiler/Profiler.h"

namespace Dark {

  class ResourceAllocator
  {
  public:
    ResourceAllocator()  = default;
    ~ResourceAllocator() = default;

    template <typename ResourceType>
    Ref<ResourceID> Load(const std::string& filePath)
    {
      for (auto it = m_Resources.begin(); it != m_Resources.end(); it++)
      {
        if (it->first->path == filePath)
          return it->first;
      }

      Ref<Resource> resource = CreateRef<ResourceType>(filePath);

      if (!resource)
      {
        return nullptr;
      }

      m_Resources.emplace(std::pair(resource->LoadFromFile(filePath)));
      return resource->GetResourceID();
    }

    template <typename ResourceType>
    Ref<ResourceID> LoadAsync(const std::string& filePath)
    {
      for (auto it = m_Resources.begin(); it != m_Resources.end(); it++)
      {
        if (it->first->path == filePath)
          return it->first;
      }

      Ref<Resource> resource = CreateRef<ResourceType>(filePath);

      if (!resource)
      {
        return nullptr;
      }

      std::pair<void*, size_t> result = FileSystem::ReadFile(filePath);

      m_Resources.emplace(std::pair(resource->LoadFromMemory(result.first, result.second)));
      return resource->GetResourceID();
    }

    Ref<ResourceID> GetResourceID(const Ref<Resource>& res);

    Ref<Resource> GetResource(const Ref<UUID>& resUuid);
    Ref<Resource> GetResource(const Ref<ResourceID> resId);

    template <typename ResourceType>
    Ref<ResourceType> GetResource(const std::string& resPath)
    {
      for (auto it = m_Resources.begin(); it != m_Resources.end(); it++)
      {
        if (it->first->path == resPath)
          return std::dynamic_pointer_cast<ResourceType>(it->second);
      }
      DK_CORE_ERROR("Get Ressource Fatal:{0}", resPath);
      return nullptr;
    }

    bool RemoveResource(const Ref<ResourceID>& resId);
    bool RemoveResource(const Ref<Resource>& res);

    bool HasRes(const UUID& resUuid);
    bool HasRes(const Ref<ResourceID> resId);
    bool HasRes(const std::string& resPath);

    size_t GetResSize() { return m_Resources.size(); }

    std::unordered_map<Ref<ResourceID>, Ref<Resource>>::iterator GetResourcesBegin()
    {
      return m_Resources.begin();
    }
  private:
    // Resources
    std::unordered_map<Ref<ResourceID>, Ref<Resource>> m_Resources;
  };

  class ResourceManager
  {
  public:
    Ref<PanelMesh> s_PanelMesh;
    Ref<ShaderLibrary> s_ShaderLibrary;

    static ResourceManager& Get()
    {
      if (m_Instance != nullptr)
        return *m_Instance;
      else
      {
        m_Instance = new ResourceManager();
        return *m_Instance;
      }
    }

    Ref<ResourceAllocator> GetResourceAllocator() { return m_ResourceAllocator; }

  protected:
    ResourceManager()
    {
      s_PanelMesh = CreateRef<PanelMesh>();
      s_ShaderLibrary = CreateRef<ShaderLibrary>();
      s_ShaderLibrary->Load("assets/shaders/Texture.glsl");

      //DK_CORE_INFO("ResourceManager()");
      m_ResourceAllocator = CreateRef<ResourceAllocator>();

      DK_CORE_TRACE("------------Resource Manager Travers Files------------")
      FileSystem::TraverseFiles("assets", [this](const std::filesystem::path& path, std::filesystem::path extension) {
#ifdef DK_PLATFORM_WINDOWS
        if (extension == ".jpg" || extension == ".png")
        {
          PROFILE_SCOPE(path.string().c_str());
          //m_ResourceAllocator->Load<Texture2D>(path.string());
          m_ResourceAllocator->LoadAsync<Texture2D>(path.string());
        }
        //if (std::string::npos != filenameString.find(".obj"))
        //{

        //}
        printf("\n");
#endif // DK_PLATFORM_WINDOWS
      });

      // Tracing
      //for (auto iter : m_ProfileResults)
      //  DK_CORE_TRACE("Profiler : load -> {0}, {1}ms", iter.Name, iter.Time);
    }

    ~ResourceManager()
    {
      //DK_CORE_INFO("~ResourceManager()");
      delete m_Instance;
    }

  private:
    static ResourceManager* m_Instance;
    // All ResourceAllocator
    Ref<ResourceAllocator> m_ResourceAllocator;
  };

} // namespace Dark