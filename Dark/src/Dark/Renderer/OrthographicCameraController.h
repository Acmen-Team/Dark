#pragma once

#include "OrthographicCamera.h"
#include "Dark/Core/Timestep.h"

#include "Dark/Events/ApplicationEvent.h"
#include "Dark/Events/MouseEvent.h"

namespace Dark {

  class OrthographicCameraController
  {
  public:
	OrthographicCameraController(float aspectRatio);

	void OnUpdate(Timestep timestep);
	void OnEvent(Event& e);

	OrthographicCamera& GetCamera() { return m_Camera; }
	const OrthographicCamera& GetCamera() const { return m_Camera; }
  private:
	bool OnMouseScrolledEvent(MouseScrolledEvent& e);
	bool OnWindowResizeEvent(WindowResizeEvent& e);
  private:
	float m_AspectRatio;
	float m_ZoomLevel = 1.0f;
	OrthographicCamera m_Camera;
  private:
	glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
	float m_CamearTransformationSpeed = 1.0;
  };

}