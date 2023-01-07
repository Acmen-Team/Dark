/*************************************************
Copyright © 2020 - 2022 YX.All Rights Reserved

Date:2022-11-5 14:00
Description:EditorCameraController
**************************************************/
#pragma once

#include <Dark.h>

namespace Dark {

  class EditorCameraController
  {
  public:
    EditorCameraController(float aspectRatio);

    void OnUpdate(Timestep timestep);
    void OnEvent(Event& e);

    Camera& GetCamera() { return m_Camera; }
    const Camera& GetCamera() const { return m_Camera; }

    glm::vec3& GetCameraPosition() { return m_CameraPosition; }
    glm::vec3& GetCameraFront() { return m_CameraFront; }

    void OnViewportResize(uint32_t width, uint32_t height);

  private:
    void UpdateCameraVectors();
    bool OnMouseMovedEvent(MouseMovedEvent& e);
    bool OnMouseScrolledEvent(MouseScrolledEvent& e);
    bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
    bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);

    void ProcessMouseMovement(float xOffset, float yOffset);
  private:
    float m_AspectRatio;
    float m_CameraTransformationSpeed = 2.0;

  private:
    Camera m_Camera;

    glm::vec3 m_CameraPosition = {0.0f, 0.0f, 3.0f};
    glm::vec3 m_CameraFront    = {0.0f, 0.0f, -1.0f};
    glm::vec3 m_CameraUp       = {0.0f, 1.0f, 0.0f};

    bool RightMouseButtonPressed = false;
    float MouseSensitivity       = 0.1f;

    bool m_FirstMouse = true;
    float lastX       = 1920 / 2;
    float lastY       = 1080 / 2;
    float m_Yaw       = -90.0f;
    float m_Pitch     = 0.0f;
  };

} // namespace Dark
