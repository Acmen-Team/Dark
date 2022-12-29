#include "dkpch.h"
#include "Math.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>

namespace Dark::Math {

  bool DecomposeTransform(const glm::mat4& transform, glm::vec3& outTranslation, glm::vec3& outRotation, glm::vec3& outScale)
  {
    using namespace glm;
    using T = float;

    mat4 LocalMatrix(transform);

    // Normalize the matrix.
    if (epsilonEqual(LocalMatrix[3][3], static_cast<float>(0), epsilon<T>()))
      return false;

    // First, isolate perspective. This is the messiest.
    if (
        epsilonEqual(LocalMatrix[0][3], static_cast<T>(0), epsilon<T>()) ||
        epsilonEqual(LocalMatrix[1][3], static_cast<T>(0), epsilon<T>()) ||
        epsilonEqual(LocalMatrix[2][3], static_cast<T>(0), epsilon<T>()))
    {
      // Clear the perspective partition
      LocalMatrix[0][3] = LocalMatrix[1][3] = LocalMatrix[2][3] = static_cast<T>(0);
      LocalMatrix[3][3]                                         = static_cast<T>(1);
    }

    // Next take care of translation.
    outTranslation = vec3(LocalMatrix[3]);
    LocalMatrix[3] = vec4(0, 0, 0, LocalMatrix[3].w);

    vec3 Row[3], Pdum3;

    // Get scale and shear.
    for (length_t i = 0; i < 3; ++i)
      for (length_t j = 0; j < 3; ++j)
        Row[i][j] = LocalMatrix[i][j];

    outScale.x = length(Row[0]);
    Row[0]     = detail::scale(Row[0], static_cast<T>(1));
    outScale.y = length(Row[1]);
    Row[1]     = detail::scale(Row[1], static_cast<T>(1));
    outScale.z = length(Row[2]);
    Row[2]     = detail::scale(Row[2], static_cast<T>(1));

#if 0
	Pdum3 = cross(Row[1], Row[2]);
	if (dot(Row[0], Pdum3) < 0)
	{
	  for (length_t i = 0; i < 3; ++i)
	  {
		outScale[i] *= static_cast<T>(-1);
		Row[i] *= static_cast<T>(-1);
	  }
	}
#endif

    outRotation.y = asin(-Row[0][2]);
    if (cos(outRotation.y) != 0)
    {
      outRotation.x = atan2(Row[1][2], Row[2][2]);
      outRotation.z = atan2(Row[0][1], Row[0][0]);
    }
    else
    {
      outRotation.x = atan2(-Row[2][0], Row[1][1]);
      outRotation.z = 0;
    }

    return true;
  }

} // namespace Dark::Math