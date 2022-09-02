#include "dkpch.h"

#include "ResourceManager.h"

namespace Dark {

  ResourceManager* ResourceManager::m_Instance = nullptr;

  Ref<ResourceID> ResourceAllocator::GetResourceID(const Ref<Resource>& res)
  {
    for (auto it = m_Resources.begin(); it != m_Resources.end(); it++)
    {
      if (it->second == res)
        return it->first;
    }
    DK_CORE_ERROR("Get Ressource Fatal:{0}", res->GetResourceID()->path);
    return nullptr;
  }

  Ref<Resource> ResourceAllocator::GetResource(const Ref<UUID>& resUuid)
  {
    for (auto it = m_Resources.begin(); it != m_Resources.end(); it++)
    {
      if (it->first->uuid == resUuid)
        return it->second;
    }
    DK_CORE_ERROR("Get Ressource Fatal:{0}", resUuid->GetUUIDString());
    return nullptr;
  }

  Ref<Resource> ResourceAllocator::GetResource(const Ref<ResourceID> resId)
  {
    try
    {
      return m_Resources.at(resId);
    }
    catch (const std::exception& e)
    {
      DK_CORE_ERROR("Get Ressource Fatal({0}):{1}", e.what(), resId->path);
      return nullptr;
    }
  }

  bool ResourceAllocator::RemoveResource(const Ref<ResourceID>& resId)
  {
    auto it = m_Resources.find(resId);
    if (it != m_Resources.end())
    {
      m_Resources.erase(resId);
      return true;
    }
    DK_CORE_ERROR("Remove Ressource Fatal:{0}", resId->path);
    return false;
  }

  bool ResourceAllocator::RemoveResource(const Ref<Resource>& res)
  {
    for (auto it = m_Resources.begin(); it != m_Resources.end(); it++)
    {
      if (it->second == res)
      {
        m_Resources.erase(it->first);
        return true;
      }
    }
    DK_CORE_ERROR("Remove Ressource Fatal:{0}", res->GetResourceID()->path);
    return false;
  }

  bool ResourceAllocator::HasRes(const UUID& resUuid)
  {
    for (auto it = m_Resources.begin(); it != m_Resources.end(); it++)
    {
      if (it->first->uuid == resUuid)
        return true;
    }
    DK_CORE_ERROR("Has Ressource Fatal:{0}", resUuid.GetUUIDString());
    return false;
  }

  bool ResourceAllocator::HasRes(const Ref<ResourceID> resId)
  {
    try
    {
      m_Resources.at(resId);
      return true;
    }
    catch (const std::exception& e)
    {
      DK_CORE_ERROR("Has Ressource Fatal({0}):{1}", e.what(), resId->path);
      return false;
    }
  }

  bool ResourceAllocator::HasRes(const std::string& resPath)
  {
    for (auto it = m_Resources.begin(); it != m_Resources.end(); it++)
    {
      if (it->first->path == resPath)
        return true;
    }
    DK_CORE_ERROR("Has Ressource Fatal:{0}", resPath);
    return false;
  }

} // namespace Dark