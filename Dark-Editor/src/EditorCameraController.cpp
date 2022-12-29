#include "EditorCameraController.h"

namespace Dark {
  EditorCameraController::EditorCameraController(float aspectRatio)
      : m_AspectRatio(aspectRatio)
  {
    UpdateCameraVectors();
  }

  void EditorCameraController::OnUpdate(Timestep timestep)
  {
    if (Input::IsKeyPressed(DK_KEY_W))
      m_CameraPosition += m_CameraTransformationSpeed * timestep.GetSeconds() * m_CameraFront;
    if (Input::IsKeyPressed(DK_KEY_S))
      m_CameraPosition -= m_CameraTransformationSpeed * timestep.GetSeconds() * m_CameraFront;
    if (Input::IsKeyPressed(DK_KEY_A))
      m_CameraPosition -= glm::normalize(glm::cross(m_CameraFront, m_CameraUp)) * m_CameraTransformationSpeed * timestep.GetSeconds();
    if (Input::IsKeyPressed(DK_KEY_D))
      m_CameraPosition += glm::normalize(glm::cross(m_CameraFront, m_CameraUp)) * m_CameraTransformationSpeed * timestep.GetSeconds();
    if (Input::IsKeyPressed(DK_KEY_E))
      m_CameraPosition += m_CameraTransformationSpeed * timestep.GetSeconds() * m_CameraUp;
    if (Input::IsKeyPressed(DK_KEY_Q))
      m_CameraPosition -= m_CameraTransformationSpeed * timestep.GetSeconds() * m_CameraUp;

    UpdateCameraVectors();
  }

  void EditorCameraController::OnEvent(Event& e)
  {
    if (m_Camera.GetProjectionType() == ProjectionType::ePerspective)
    {
      EventDispatcher dispatcher(e);
      dispatcher.Dispatch<MouseMovedEvent>(DK_BIND_EVENT_FN(EditorCameraController::OnMouseMovedEvent));
      dispatcher.Dispatch<MouseScrolledEvent>(DK_BIND_EVENT_FN(EditorCameraController::OnMouseScrolledEvent));
      dispatcher.Dispatch<MouseButtonPressedEvent>(DK_BIND_EVENT_FN(EditorCameraController::OnMouseButtonPressedEvent));
      dispatcher.Dispatch<MouseButtonReleasedEvent>(DK_BIND_EVENT_FN(EditorCameraController::OnMouseButtonReleasedEvent));
    }
  }

  void EditorCameraController::OnViewportResize(uint32_t width, uint32_t height)
  {
    if (!m_Camera.fixedAspectration)
      m_Camera.OnViewportResize(width, height);
  }

  void EditorCameraController::UpdateCameraVectors()
  {
    m_CameraFront.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
    m_CameraFront.y = sin(glm::radians(m_Pitch));
    m_CameraFront.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
    m_CameraFront   = glm::normalize(m_CameraFront);

    glm::vec3 right = glm::normalize(glm::cross(m_CameraFront, glm::vec3(0.0f, 1.0f, 0.0f)));
    m_CameraUp      = glm::normalize(glm::cross(right, m_CameraFront));

    m_Camera.SetViewMatrix(glm::lookAt(m_CameraPosition, m_CameraPosition + m_CameraFront, m_CameraUp));
  }

  bool EditorCameraController::OnMouseMovedEvent(MouseMovedEvent& e)
  {
    if (RightMouseButtonPressed)
    {
      if (m_FirstMouse)
      {
        lastX = e.GetX();
        lastY = e.GetY();

        m_FirstMouse = false;
      }

      float xoffset = e.GetX() - lastX;
      float yoffset = lastY - e.GetY();

      lastX = e.GetX();
      lastY = e.GetY();

      ProcessMouseMovement(xoffset, yoffset);
    }

    return false;
  }

  bool EditorCameraController::OnMouseScrolledEvent(MouseScrolledEvent& e)
  {
    if (RightMouseButtonPressed)
    {
      //if (m_Fov >= 1.0f && m_Fov <= 45.0f)
      //	m_Fov -= e.GetYOffset();
      //if (m_Fov <= 1.0f)
      //	m_Fov = 1.0f;
      //if (m_Fov >= 45.0f)
      //	m_Fov = 45.0f;
      //m_Camera.SetProjectionMatrix(m_Fov, m_AspectRatio, m_NearPlan, m_FarPlan);
    }
    else
    {
      m_CameraPosition += (0.1f * e.GetYOffset()) * m_CameraFront;
      UpdateCameraVectors();
    }

    return false;
  }

  bool EditorCameraController::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e)
  {
    if (e.GetMouseButton() == DK_MOUSE_BUTTON_RIGHT)
      RightMouseButtonPressed = true;
    
    return false;
  }

  bool EditorCameraController::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e)
  {
    RightMouseButtonPressed = false;
    m_FirstMouse            = true;

    return false;
  }

  void EditorCameraController::ProcessMouseMovement(float xOffset, float yOffset)
  {
    xOffset *= MouseSensitivity;
    yOffset *= MouseSensitivity;

    m_Yaw += xOffset;
    m_Pitch += yOffset;

    if (m_Pitch > 89.9f)
      m_Pitch = 89.9f;
    if (m_Pitch < -89.9f)
      m_Pitch = -89.9f;

    UpdateCameraVectors();
  }

} // namespace Dark
