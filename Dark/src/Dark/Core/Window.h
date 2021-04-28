/*************************************************
Copyright © 2020 - 2020 YX.All Rights Reserved

Date:2020-12-27 16:25
Description:Window base class 
**************************************************/
#pragma once

#include "dkpch.h"

#include "Core.h"

#include "Dark/Events/Event.h"

namespace Dark {
    
  struct WindowProps
  {
	std::string Title;
	unsigned int Width;
	unsigned int Height;

	WindowProps(const std::string& title = "Dark Engine",
				unsigned int width = 1280,
				unsigned int height = 720) 
			  :Title(title), Width(width), Height(height) {}
  };

  // Interface representing a desktop system based Window
  class DARK_API Window
  {
  public:
	using EventCallbackFn = std::function<void(Event&)>;

	virtual ~Window() {}

	virtual void OnUpdate() = 0;

	virtual void* GetNativeWindow() const = 0;
	virtual unsigned int GetWidth() const = 0;
	virtual unsigned int GetHeight() const = 0;

	// Window attributes
	virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
	virtual void SetVSync(bool enabled) = 0;
	virtual bool IsVSync() const = 0;

	static Window* Create(const WindowProps& props = WindowProps());
  };

}