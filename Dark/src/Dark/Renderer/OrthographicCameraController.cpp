#include "dkpch.h"

#include "OrthographicCameraController.h"

#include "Dark/Core/Input.h"
#include "Dark/Core/KeyCodes.h"
#include "Dark/Core/MouseButtonCodes.h"

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

}