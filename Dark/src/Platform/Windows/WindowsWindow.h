/*************************************************
Copyright © 2020 - 2020 YX.All Rights Reserved

Date:2020-12-27 16:40
Description:Windows PlatForm Window class
**************************************************/
#pragma once

#include "Dark/Core/Window.h"
#include "Dark/Renderer/RendererContext.h"

#include "glad/glad.h"
#include <GLFW/glfw3.h>

namespace Dark {

  class WindowsWindow :public Window
  {
  public:
	WindowsWindow(const WindowProps& props);
	virtual ~WindowsWindow();

	void OnUpdate() override;
	
	virtual void* GetNativeWindow() const override { return m_Window; }
	inline unsigned int GetWidth() const override { return m_Data.Width; }
	inline unsigned int GetHeight() const override { return m_Data.Height; }

	// Window attribute
	inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
	void SetVSync(bool enabled) override;
	bool IsVSync() const override;
  private:
	virtual void Init(const WindowProps& props);
	virtual void Shutdown();
  private:
	GLFWwindow* m_Window;
	RenderContext *m_Context;

	struct WindowData
	{
	  std::string Title;
	  unsigned int Width, Height;
	  bool VSync;

	  EventCallbackFn EventCallback;
	};

	WindowData m_Data;
  };


}
