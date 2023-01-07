/*************************************************
Copyright © 2020 - 2022 YX.All Rights Reserved

Date:2022-8-14 15:38
Description:Unique ids
**************************************************/
#pragma once

// stduuid library:https://github.com/mariusbancila/stduuid
// Windows platform UUID:https://docs.microsoft.com/en-us/windows/win32/api/rpcdce/nf-rpcdce-uuidcreate

#define UUID_SYSTEM_GENERATOR
#include <uuid.h>

namespace Dark {

  class UUID
  {
  public:
    UUID();
    
    UUID(const std::string& url);

    std::string ConvertUUIDToString() const { return uuids::to_string(m_Uuid); }

    size_t ConvertUUIDToHash() const { return m_HashFunc(m_Uuid); }

    bool const operator==(const Ref<UUID>& uuid) const
    {
      return uuid->m_Uuid == m_Uuid;
    }

  private:
    uuids::uuid m_Uuid;
    std::hash<uuids::uuid> m_HashFunc;
  };

} // namespace Dark