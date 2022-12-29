/*************************************************
Copyright © 2020 - 2020 YX.All Rights Reserved

Date:2020-12-27 16:40
Description:Windows PlatForm Window class
**************************************************/
#pragma once

#include "Dark/Core/Window.h"
#include "Dark/Renderer/RendererContext.h"

#include "glad/glad.h"
#define GLFW_INCLUDE_VULKAN
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#define GLFW_NATIVE_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

namespace Dark {

  class WindowsWindow : public Window
  {
  public:
    WindowsWindow(const WindowProps& props);
    virtual ~WindowsWindow();

    virtual void SetWindowAttrib() override;
    virtual void SetMinimize() override;
    virtual void SetMaximizeOrRestore() override;

    virtual void OnUpdate() override;

    virtual void* GetNativeWindow() const override { return m_Window; }
    virtual inline float GetPosX() const override { return m_Data.PosX; }
    virtual inline float GetPosY() const override { return m_Data.PosY; }
    virtual inline unsigned int GetWidth() const override { return m_Data.Width; }
    virtual inline unsigned int GetHeight() const override { return m_Data.Height; }

    // Window attribute
    inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
    virtual void SetVSync(bool enabled) override;
    virtual bool IsVSync() const override;
  private:
    virtual void Init(const WindowProps& props);
    virtual void Shutdown();

  private:
    GLFWwindow* m_Window;
    RenderContext* m_Context;
    VkInstance m_Instance;

    struct WindowData
    {
      std::string Title;
      unsigned int Width, Height;
      float PosX, PosY;
      bool VSync;

      EventCallbackFn EventCallback;
    };

    WindowData m_Data;
  };

} // namespace Dark
