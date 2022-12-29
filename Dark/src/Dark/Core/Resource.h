/*************************************************
Copyright © 2020 - 2022 YX.All Rights Reserved

Date:2022-8-14 16:35
Description:Resource base class
**************************************************/
#pragma once
#include "Dark/Core/Core.h"

#include "Dark/Core/UUid.h"

namespace Dark {

  enum class ResourceType
  {
    eMesh    = 0,
    eTexture = 1,
    eShader  = 2,

    eMax = -1
  };

  enum ResourceFlag
  {
    LocalFile = 0,
  };

  struct ResourceID
  {
    std::string name;
    std::string path;
    //const void* type{};
    ResourceType type{-1};
    Ref<UUID> uuid;
    //std::list<ResourceID> dependcy{};
    //ResourceFlag flag{};
    time_t timep;

  public:
    ResourceID(const std::filesystem::path& respath)
    {
      auto relativePath = std::filesystem::relative(respath, "assets");
      name              = relativePath.filename().string();
      DK_CORE_INFO("Resource Name:{0}", name);
      path = respath.string();
      DK_CORE_INFO("Resource Path:{0}", path);
      uuid = CreateRef<UUID>(path);
      DK_CORE_INFO("Resource UUID:{0}", uuid->ConvertUUIDToString());
      time(&timep);
      std::string str     = std::string(ctime(&timep));
      str[str.size() - 1] = '/0';
      DK_CORE_INFO("Resource Create Time:{0}", str.c_str());
    }
  };

  class Resource
  {
  public:
    Resource()
    {
    }

    virtual ~Resource() = default;

    Ref<ResourceID> GetResourceID()
    {
      return m_ResID;
    }

    Ref<Resource> GetResource()
    {
      return m_Res;
    }

    virtual std::pair<Ref<ResourceID>, Ref<Resource>> LoadFromFile(const std::string& path)  = 0;
    virtual std::pair<Ref<ResourceID>, Ref<Resource>> LoadFromMemory(void* mem, size_t size) = 0;

  protected:
    // TODO: 存在循环，m_Res->m_ResID为空，真正的ID在this->m_ResID
    Ref<Resource> m_Res{nullptr};
    Ref<ResourceID> m_ResID{nullptr};
  };

} // namespace Dark