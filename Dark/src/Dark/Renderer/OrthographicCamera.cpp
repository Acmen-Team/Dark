#include "dkpch.h"

#include "OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Dark {

  OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
      : m_ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), m_ViewMatrix(1.0f)
  {
    m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
  }

  void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
  {
    m_ProjectionMatrix     = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
    m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
  }

  void OrthographicCamera::RecalculateViewMatrix()
  {
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) * glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));

    m_ViewMatrix           = glm::inverse(transform);
    m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
  }

  CameraBookmark OrthographicCamera::SetCameraBookmark(const glm::vec3 position)
  {
    CameraBookmark bookmark;
    for (int i = 0; i < BOOKMARK_SIZE; ++i)
    {
      if (!m_CameraBookmark[i].m_Flag)
      {
        bookmark            = CameraBookmark(i, std::to_string(i), position, true);
        m_CameraBookmark[i] = bookmark;
        return bookmark;
      }
    }
    return bookmark;
  }

  void OrthographicCamera::DeleteCameraBookmark(const CameraBookmark bookmark)
  {
    for (int i = 0; i < BOOKMARK_SIZE; ++i)
    {
      if (bookmark.m_Flag && m_CameraBookmark[i].m_Id == bookmark.m_Id)
      {
        m_CameraBookmark[i].m_Flag = false;
        break;
      }
    }
  }

  CameraBookmark OrthographicCamera::GetCameraBookmark(const int index)
  {
    CameraBookmark bookmark;
    if (index >= 0 && index < BOOKMARK_SIZE)
    {
      bookmark = m_CameraBookmark[index];
    }
    return bookmark;
  }

} // namespace Dark