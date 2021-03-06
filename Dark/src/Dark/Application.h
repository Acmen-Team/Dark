#pragma once

#include "Core.h"

#include "Window.h"
#include "Dark/LayerStack.h"
#include "Dark/Events/Event.h"
#include "Dark/Events/ApplicationEvent.h"

#include "Dark/ImGui/ImGuiLayer.h"

#include "Dark/Renderer/Shader.h"
#include "Dark/Renderer/Buffer.h"
#include "Dark/Renderer/VertexArray.h"
#include "Dark/Renderer/OrthographicCamera.h"

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

	inline Window& GetWindow() { return *m_Window; }
	inline static Application& Get() { return *m_Instance; }
  private:
	bool OnWindowClose(WindowCloseEvent& e);

	std::unique_ptr<Window> m_Window;
	ImGuiLayer* m_ImGuiLayer;
	bool m_Running = true;
	LayerStack m_LayerStack;

	float m_LastFramTime = 0.0f;
  private:
	static Application* m_Instance;
  };

  // define in CLIENT
  Application* CreateApplication();
}

