#pragma once

#include "Core.h"

#include "Window.h"
#include "Dark/LayerStack.h"
#include "Dark/Events/Event.h"
#include "Dark/Events/ApplicationEvent.h"

#include "Dark/ImGui/ImGuiLayer.h"

namespace Dark {
  
  class DARK_API Application
  {
  public:
	Application(const std::string& name = "DarK");
	virtual ~Application();

	void StartUp();
	void OnEvent(Event& e);

	void PushLayer(Layer* layer);
	void PushOverlay(Layer* layer);

	void Exit();

	inline Window& GetWindow() { return *m_Window; }
	inline static Application& Get() { return *m_Instance; }
  private:
	bool OnWindowClose(WindowCloseEvent& e);

	std::unique_ptr<Window> m_Window;
	ImGuiLayer* m_ImGuiLayer;
	bool m_Running = true;
	LayerStack m_LayerStack;
  private:
	static Application* m_Instance;
  };

  // define in CLIENT
  Application* CreateApplication();
}

