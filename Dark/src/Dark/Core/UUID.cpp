/*************************************************
Copyright © 2020 - 2022 YX.All Rights Reserved

Date:2022-8-14 16:14
Description:Uuid
**************************************************/
#include "dkpch.h"

#include "UUid.h"

namespace Dark {

  UUID::UUID()
  {
    m_Uuid = uuids::uuid_system_generator{}();
  }

  UUID::UUID(const std::string& url)
  {
    if (uuids::uuid::is_valid_uuid(url))
    {
      m_Uuid = uuids::uuid::from_string(url).value();
    }
    else
    {
      uuids::uuid_name_generator gen(uuids::uuid::from_string("47183823-2574-4bfd-b411-99ed177d3e43").value());
      m_Uuid = gen(url);
    }

    //DK_CORE_INFO("{0}", !m_Uuid.is_nil());
    //DK_CORE_INFO("{0}", m_Uuid.version() == uuids::uuid_version::name_based_sha1);
    //DK_CORE_INFO("{0}", m_Uuid.variant() == uuids::uuid_variant::rfc);
    //DK_CORE_INFO("{0}", uuids::to_string(m_Uuid));
  }

} // namespace Dark
