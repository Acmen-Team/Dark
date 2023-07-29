#pragma once

#include <glm/glm.hpp>

namespace Dark {

  struct LightProperty
  {
    glm::vec4 Color;
    float Intensity;
    float Range;
    float SpotAngle;

    int LightType;

    LightProperty()
    {
      Color     = glm::vec4(0.2f, 0.2f, 0.3f, 1.0f);
      Range     = 5;
      Intensity = 1.0;
      SpotAngle = 15;

      LightType = 0;
    }
  };

} // namespace Dark