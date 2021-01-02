﻿/*************************************************
Copyright © 2020 - 2020 YX.All Rights Reserved

Date:2020-12-27 16:49
Description:Windows Platform Window class
**************************************************/
#include "dkpch.h"
#include "WindowsWindow.h"

#include "Dark/Events/ApplicationEvent.h"
#include "Dark/Events/KeyEvent.h"
#include "Dark/Events/MouseEvent.h"

namespace Dark {

  static bool s_GLFWInitialized = false;

  static void GLFWErrorCallback(int error, const char* description)
  {
	DK_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
  }

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

	  glfwSetErrorCallback(GLFWErrorCallback);
	  s_GLFWInitialized = true;
	}

	m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
	glfwMakeContextCurrent(m_Window);
	glfwSetWindowUserPointer(m_Window, &m_Data);
	SetVSync(true);

	// Set GLFW callbacks
	glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
	{
	  WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
	  data.Width = width;
	  data.Height = height;

	  WindowResizeEvent event(width, height);
	  data.EventCallback(event);
	});

	glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
	{
	  WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
	  WindowCloseEvent event;
	  data.EventCallback(event);
	});

	glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
	{
	  WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

	  switch (action)
	  {
		case GLFW_PRESS:
		{
		  KeyPressedEvent event(key, 0);
		  data.EventCallback(event);
		  break;
		}
		case GLFW_RELEASE:
		{
		  KeyReleasedEvent event(key);
		  data.EventCallback(event);
		  break;
		}
		case GLFW_REPEAT:
		{
		  KeyPressedEvent event(key, 1);
		  data.EventCallback(event);
		  break;
		}
	  }
	});

	glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
	{
	  WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

	  switch (action)
	  {
		case GLFW_PRESS:
		{
		  MouseButtonPressedEvent event(button);
		  data.EventCallback(event);
		  break;
		}
		case GLFW_RELEASE:
		{
		  MouseButtonReleasedEvent event(button);
		  data.EventCallback(event);
		  break;
		}
	  }
	});

	glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
	{
	  WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

	  MouseScrolledEvent event((float)xOffset, (float)yOffset);
	  data.EventCallback(event);
	});

	glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
	{
	  WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

	  MouseMovedEvent event((float)xPos, (float)yPos);
	  data.EventCallback(event);
	});

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
