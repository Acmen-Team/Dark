/*************************************************
Copyright © 2020 - 2022 YX.All Rights Reserved

Date:2022-8-14 16:35
Description:Resource base class
**************************************************/
#pragma once
#include "Dark/Core/Core.h"

#include "Dark/Core/UUid.h"

namespace Dark {

  //enum ResourceType
  //{
  //  Mesh    = 0,
  //  Texture = 1,
  //  Shader  = 2,
  //};

  enum ResourceFlag
  {
    LocalFile = 0,
  };

  struct ResourceID
  {
    std::string name;
    std::string path;
    const void* type{};
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
      path              = respath.string();
      DK_CORE_INFO("Resource Path:{0}", path);
      uuid              = CreateRef<UUID>(path);
      DK_CORE_INFO("Resource UUID:{0}", uuid->GetUUIDString());
      time(&timep);
      DK_CORE_INFO("Resource Create Time:{0}", ctime(&timep));
    }
  };

  class Resource
  {
  public:
    Resource()
    {
      //DK_CORE_INFO("Resource()");
    }
    virtual ~Resource() = default;

    Ref<Resource> GetResource()
    {
      return m_Res;
    }
    Ref<ResourceID> GetResourceID()
    {
      return m_ResID;
    }

    virtual std::pair<Ref<ResourceID>, Ref<Resource>> LoadFromFile(const std::string& path) = 0;

  protected:
    Ref<Resource> m_Res{nullptr};
    Ref<ResourceID> m_ResID{nullptr};
  };

} // namespace Dark