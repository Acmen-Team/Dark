#pragma once

#include "Core.h"
#include "Window.h"
#include "LayerStack.h"

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
    void PopBottomLayer();

    void Exit();

    inline Window& GetWindow() { return *m_Window; }
    inline static Application& Get() { return *m_Instance; }
    
    bool IsRead() { return m_InitFunc._Is_ready(); }
  private:
    bool OnWindowClose(WindowCloseEvent& e);
    bool OnWindowResize(WindowResizeEvent& e);

    std::unique_ptr<Window> m_Window;
    ImGuiLayer* m_ImGuiLayer;
    bool m_Running   = true;
    bool m_Minimized = false;
    LayerStack m_LayerStack;

    float m_LastFramTime = 0.0f;

  private:
    static Application* m_Instance;

    std::future<void> m_InitFunc;
  };

  // define in CLIENT
  Application* CreateApplication();
} // namespace Dark
