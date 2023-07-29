#include "dkpch.h"
#include "Material.h"

namespace Dark {

  MaterialData* Material::Create()
  {
    return new MaterialData();
  }

} // namespace Dark