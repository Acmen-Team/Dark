/*************************************************
Copyright © 2020 - 2022 YX.All Rights Reserved

Date:2022-8-14 15:17
Description:ResourceManager
**************************************************/
#pragma once

#include "Dark/Resource/Texture.h"
#include "Dark/Core/IO/FileSystem.h"

namespace Dark {

  class ResourceAllocator
  {
  public:
    ResourceAllocator()  = default;
    ~ResourceAllocator() = default;

    template <typename ResourceType>
    Ref<ResourceID> LoadFromFile(const std::string& filePath)
    {
      for (auto it = m_Resources.begin(); it != m_Resources.end(); it++)
      {
        if (it->first->path == filePath)
          return it->first;
      }

      Ref<Resource> resource = CreateRef<ResourceType>();

      if (!resource)
      {
        return nullptr;
      }

      m_Resources.emplace(std::pair(resource->LoadFromFile(filePath)));
      return resource->GetResourceID();
    }

    template <typename ResourceType>
    Ref<std::future<Ref<ResourceID>>> LoadFromFileAsync(const std::string& filePath)
    {
      return CreateRef(std::async(std::launch::async, ResourceAllocator::LoadFromFile, this));
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

  private:
    // Resources
    std::unordered_map<Ref<ResourceID>, Ref<Resource>> m_Resources;
  };

  class ResourceManager
  {
  public:
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
      //m_Instance = this;
      //while (true)
      //{
      //  DK_CORE_INFO("while");
      //}

      //DK_CORE_INFO("ResourceManager()");
      m_ResourceAllocator = CreateRef<ResourceAllocator>();

      FileSystem::TraverseFiles("assets", [this](const std::filesystem::path& path, std::filesystem::path extension) {
#ifdef DK_PLATFORM_WINDOWS
        if (extension == ".jpg" || extension == ".png")
          m_ResourceAllocator->LoadFromFile<Texture2D>(path.string());
          //if (std::string::npos != filenameString.find(".obj"))

#endif // DK_PLATFORM_WINDOWS
      });
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