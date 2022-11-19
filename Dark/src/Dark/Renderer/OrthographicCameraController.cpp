#include "dkpch.h"

#include "OrthographicCameraController.h"

#include "Dark/Core/Input.h"
#include "Dark/Core/KeyCodes.h"
#include "Dark/Core/MouseButtonCodes.h"
#include "Dark/Core/Application.h"
#include <GLFW/glfw3.h>

namespace Dark {

  OrthographicCameraController::OrthographicCameraController(float aspectRatio)
      : m_AspectRatio(aspectRatio), m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel)
  {

  }

  void OrthographicCameraController::OnUpdate(Timestep timestep)
  {
    if (Input::IsKeyPressed(DK_KEY_D))
      m_CameraPosition.x -= m_CamearTransformationSpeed * timestep.GetSeconds() * (m_ZoomLevel / (m_ZoomLevel + 1));
    if (Input::IsKeyPressed(DK_KEY_A))
      m_CameraPosition.x += m_CamearTransformationSpeed * timestep.GetSeconds() * (m_ZoomLevel / (m_ZoomLevel + 1));
    if (Input::IsKeyPressed(DK_KEY_W))
      m_CameraPosition.y -= m_CamearTransformationSpeed * timestep.GetSeconds() * (m_ZoomLevel / (m_ZoomLevel + 1));
    if (Input::IsKeyPressed(DK_KEY_S))
      m_CameraPosition.y += m_CamearTransformationSpeed * timestep.GetSeconds() * (m_ZoomLevel / (m_ZoomLevel + 1));

    m_Camera.SetPosition(m_CameraPosition);

    m_CamearTransformationSpeed = m_ZoomLevel;
  }

  void OrthographicCameraController::OnEvent(Event& e)
  {
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<MouseScrolledEvent>(DK_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolledEvent));
    dispatcher.Dispatch<WindowResizeEvent>(DK_BIND_EVENT_FN(OrthographicCameraController::OnWindowResizeEvent));
    dispatcher.Dispatch<KeyPressedEvent>(DK_BIND_EVENT_FN(OrthographicCameraController::OnKeyPressedEvent));
  }

  bool OrthographicCameraController::OnKeyPressedEvent(KeyPressedEvent& e)
  {
    // Recording camera bookmark
    if (e.GetKeyCode() == DK_KEY_F)
    {
      glm::vec3 position           = GetCameraPosition();
      m_CurrentCameraBookmark.m_Id = GetCamera().SetCameraBookmark(position).m_Id;
    }

    // The current camera bookmark will only be deleted if the current location is any bookmark location
    if (Input::IsKeyPressed(DK_KEY_C))
    {
      m_Camera.DeleteCameraBookmark(m_CurrentCameraBookmark);
    }

    // After moving the camera
    // The current camera bookmark was setted to disable
    if (Input::IsKeyPressed(DK_KEY_D) ||
        Input::IsKeyPressed(DK_KEY_A) ||
        Input::IsKeyPressed(DK_KEY_W) ||
        Input::IsKeyPressed(DK_KEY_S))
    {
      SetCurrentCameraBookmark(-1, {}, false);
    }

    // Loading camera bookmark
    if (Input::IsKeyPressed(DK_KEY_F1))
      LoadCurrentCameraBookmark(0);
    if (Input::IsKeyPressed(DK_KEY_F2))
      LoadCurrentCameraBookmark(1);
    if (Input::IsKeyPressed(DK_KEY_F3))
      LoadCurrentCameraBookmark(2);

    return false;
  }

  bool OrthographicCameraController::OnMouseScrolledEvent(MouseScrolledEvent& e)
  {
    m_ZoomLevel -= e.GetYOffset() * 0.25f;
    m_ZoomLevel = std::max(m_ZoomLevel, 0.2f);
    m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);

    return false;
  }

  bool OrthographicCameraController::OnWindowResizeEvent(WindowResizeEvent& e)
  {
    m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
    m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);

    return false;
  }

  void OrthographicCameraController::SetCurrentCameraBookmark(const int id, const glm::vec3 position, bool flag)
  {
    m_CurrentCameraBookmark.m_Id       = id;
    m_CurrentCameraBookmark.m_Bookmark = position;
    m_CurrentCameraBookmark.m_Name     = std::to_string(id);
    m_CurrentCameraBookmark.m_Flag     = flag;
  }

  void OrthographicCameraController::LoadCurrentCameraBookmark(const int index)
  {
    CameraBookmark bookmark = m_Camera.GetCameraBookmark(index);
    // The camera position is set only when the bookmark is enabled
    if (bookmark.m_Flag)
    {
      m_CurrentCameraBookmark = bookmark;
      m_CameraPosition        = bookmark.m_Bookmark;
    }
  }

} // namespace Dark