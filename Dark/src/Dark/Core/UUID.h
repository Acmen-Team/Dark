/*************************************************
Copyright © 2020 - 2022 YX.All Rights Reserved

Date:2022-8-14 15:38
Description:Unique ids
**************************************************/
#pragma once

// stduuid library:https://github.com/mariusbancila/stduuid
// Windows platform UUID:https://docs.microsoft.com/en-us/windows/win32/api/rpcdce/nf-rpcdce-uuidcreate

#include <uuid.h>

namespace Dark {

  class UUID
  {
  public:
    /*
    UUID(const std::string& name)
    {
      uuids::uuid_name_generator gen("dsad");
    }
    */
    UUID(const std::string& url);

    std::string GetUUIDString() const { return uuids::to_string(m_Uuid); }

    bool const operator==(const Ref<UUID>& uuid) const
    {
      return uuid->m_Uuid == m_Uuid;
    }

  private:
    uuids::uuid m_Uuid;
  };

} // namespace Dark