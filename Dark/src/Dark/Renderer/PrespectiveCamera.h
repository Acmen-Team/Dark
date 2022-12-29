#pragma once

#include <glm/glm.hpp>

namespace Dark {

  class PrespectiveCamera
  {
  public:
    PrespectiveCamera(float fov, float width, float height, float nearPlan, float farPlan);

    const glm::vec3& GetPosition() const { return m_Position; }
    void SetPosition(const glm::vec3& position)
    {
      m_Position = position;
      RecalculateViewMatrix();
    }
    glm::vec3 GetRotation() const { return m_Rotation; }
    void SetRotation(const glm::vec3& rotation)
    {
      m_Rotation = rotation;
      RecalculateViewMatrix();
    }

    const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
    const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
    const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

  private:
    void RecalculateViewMatrix();

  private:
    glm::mat4 m_ProjectionMatrix;
    glm::mat4 m_ViewMatrix;
    glm::mat4 m_ViewProjectionMatrix;

    glm::vec3 m_Position = {0.0f, 0.0f, 0.0f};
    glm::vec3 m_Rotation = {0.0f, 0.0f, 0.0f};
  };

} // namespace Dark