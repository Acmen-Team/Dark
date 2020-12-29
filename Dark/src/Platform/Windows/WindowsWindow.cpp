﻿/*************************************************
Copyright © 2020 - 2020 YX.All Rights Reserved

Date:2020-12-27 16:49
Description:Windows Platform Window class 
**************************************************/
#include "dkpch.h"
#include "WindowsWindow.h"

namespace Dark {

  static bool s_GLFWInitialized = false;

  Window* Window::Create(const WindowProps& props)
  {
	return new WindowsWindow(props);
  }

  WindowsWindow::WindowsWindow(const WindowProps& props)
  {
	Init(props);
  }

  WindowsWindow::~WindowsWindow()
  {
	Shutdown();
  }

  void WindowsWindow::Init(const WindowProps& props)
  {
	m_Data.Title = props.Title;
	m_Data.Width = props.Width;
	m_Data.Height = props.Height;

	DK_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

	if (!s_GLFWInitialized)
	{
	  int success = glfwInit();
	  DK_CORE_ASSERT(success, "Could not initialize GLFW!");

	  s_GLFWInitialized = true;
	}

	m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
	glfwMakeContextCurrent(m_Window);
	glfwSetWindowUserPointer(m_Window, &m_Data);
	SetVSync(true);
  }

  void WindowsWindow::OnUpdate()
  {
	glfwPollEvents();
	glfwSwapBuffers(m_Window);
  }

  void WindowsWindow::Shutdown()
  {
	glfwDestroyWindow(m_Window);
  }

  void WindowsWindow::SetVSync(bool enabled)
  {
	if (enabled)
	  glfwSwapInterval(1);
	else
	  glfwSwapInterval(0);

	m_Data.VSync = enabled;
  }

  bool WindowsWindow::IsVSync() const
  {
	return m_Data.VSync;
  }

}
