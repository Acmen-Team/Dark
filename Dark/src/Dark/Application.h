#pragma once

#include "Core.h"

#include "Window.h"
#include "Dark/LayerStack.h"
#include "Dark/Events/Event.h"
#include "Dark/Events/ApplicationEvent.h"

namespace Dark {

  class DARK_API Application
  {
  public:
	Application();
	virtual ~Application();

	void StartUp();
	void OnEvent(Event& e);

	void PushLayer(Layer* layer);
	void PushOverlay(Layer* layer);
  private:
	bool OnWindowClose(WindowCloseEvent& e);

	std::unique_ptr<Window> m_Window;
	bool m_Running = true;
	LayerStack m_LayerStack;
  };

  // define in CLIENT
  Application* CreateApplication();
}

