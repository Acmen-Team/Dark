/*************************************************
Copyright © 2020 - 2021 YX.All Rights Reserved

Date:2021-1-24 23:11
Description:ImGuiLayer
**************************************************/
#pragma once

#include "Dark/Layer.h"

#include "Dark/Events/ApplicationEvent.h"
#include "Dark/Events/KeyEvent.h"
#include "Dark/Events/MouseEvent.h"

namespace Dark {

  class DARK_API ImGuiLayer : public Layer
  {
  public:
	ImGuiLayer();
	~ImGuiLayer();

	void OnAttach();
	void OnDetach();
	void OnUpdate();
	void OnEvent(Event& event);
  protected:
  private:
	bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
	bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
	bool OnMouseMovedEvent(MouseMovedEvent& e);
	bool OnMouseScrolledEvent(MouseScrolledEvent& e);
	bool OnKeyPressedEvent(KeyPressedEvent& e);
	bool OnKeyReleasedEvent(KeyReleasedEvent& e);
	bool OnKeyTypedEvent(KeyTypedEvent& e);
	bool OnWindowResizeEvent(WindowResizeEvent& e);
  private:
	float m_Time = 0.0f;
  };

}