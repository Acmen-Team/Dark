#pragma once

#include "Dark/Resource/Texture.h"

namespace Dark {

  template <typename ResourceType>
  class ResourceAllocator
  {
  public:
    ResourceAllocator()  = default;
    ~ResourceAllocator() = default;

    int AddRes(const std::string& filePath)
    {
      auto it = m_Resources.find(filePath);
      if (it != m_Resources.end())
      {
        return it->second.first;
      }

      Ref<ResourceType> resource = ResourceType::LoadFromFile(filePath);

      if (!resource)
      {
        return -1;
      }

      m_Resources.insert(std::make_pair(filePath, std::make_pair(m_CurrentId, resource)));

      return m_CurrentId++;
    }

    void RemoveRes(int id)
    {
      for (auto it = m_Resources.begin(); it != m_Resources.end(); ++it)
      {
        if (id == it->second.first)
          m_Resources.erase(it->first);
      }
    }

    int GetResId(const Ref<ResourceType>& mesh)
    {
      for (auto res : m_Resources)
      {
        if (mesh == res.second.second)
          return res.second.first;
      }

      return -1;
    }

    Ref<ResourceType> GetRes(int id)
    {
      for (auto it = m_Resources.begin(); it != m_Resources.end(); ++it)
      {
        if (id == it->second.first)
        {
          return it->second.second;
        }
      }
      return nullptr;
    }

    Ref<ResourceType> GetRes(const std::string& filePath)
    {
      auto it = m_Resources.find(filePath);
      if (it != m_Resources.end())
        return it->second.second;
      else
        return nullptr;
    }

    bool HasRes(int id)
    {
      return (GetRes(id) != nullptr);
    }

    int GetResSize() { return m_CurrentId; }

  private:
    // Resource id -> ToDo: use UUID
    int m_CurrentId = 0;
    // Resources
    std::map<std::string, std::pair<int, Ref<ResourceType>>> m_Resources;
  };

  class ResourceManager
  {
  public:
    static ResourceManager& Get() { return m_Instance; }

    Ref<ResourceAllocator<Texture2D>> GetTextureAllocator() { return m_TextureAllocator; }
  protected:
    ResourceManager() 
    {
      m_TextureAllocator = CreateRef<ResourceAllocator<Texture2D>>();
    }
    ~ResourceManager() {}
  private:
    static ResourceManager m_Instance;
    // All ResourceAllocator
   Ref<ResourceAllocator<Texture2D>> m_TextureAllocator;
   Ref<ResourceAllocator<Texture>> m_TAllocator;
  };

} // namespace Dark