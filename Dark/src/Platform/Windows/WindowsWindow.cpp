/*************************************************
Copyright © 2020 - 2020 YX.All Rights Reserved

Date:2020-12-27 16:49
Description:Windows Platform Window class
**************************************************/
#include "dkpch.h"
#include "WindowsWindow.h"

#include "Dark/Events/ApplicationEvent.h"
#include "Dark/Events/KeyEvent.h"
#include "Dark/Events/MouseEvent.h"
#include "Dark/Events/InputEvent.h"
#include "Platform/OpenGL/OpenGLContext.h"

#include <stb_image.h>

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
    m_Data.Title  = props.Title;
    m_Data.Width  = props.Width;
    m_Data.Height = props.Height;

    DK_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

    if (!s_GLFWInitialized)
    {
      int success = glfwInit();
      DK_CORE_ASSERT(success, "Could not initialize GLFW!");

      glfwSetErrorCallback(GLFWErrorCallback);
      s_GLFWInitialized = true;
    }

    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_CONTEXT_RELEASE_BEHAVIOR, GLFW_RELEASE_BEHAVIOR_FLUSH);
#ifdef DK_RENDER_VULKAN
    VkApplicationInfo appInfo{};
    appInfo.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName   = "Dark Vulkan";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName        = "Dark Engine";
    appInfo.engineVersion      = VK_MAKE_VERSION(0, 2, 0);
    appInfo.apiVersion         = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType            = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;

    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    createInfo.enabledExtensionCount   = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;
    createInfo.enabledLayerCount       = 0;

    if (vkCreateInstance(&createInfo, nullptr, &m_Instance) != VK_SUCCESS)
    {
      DK_CORE_ERROR("failed to create vulkan instance!");
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);

    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    DK_CORE_INFO("{0}:extensions supported", extensionCount);

    std::vector<VkExtensionProperties> extensions(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

    for (const auto& extension : extensions)
    {
      DK_CORE_TRACE("{0}", extension.extensionName);
    }
#else
    m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);

    glfwMakeContextCurrent(m_Window);
    gladLoadGL();
#endif // DK_RENDER_VULKAN

    /*  HWND nativeWindow = glfwGetWin32Window(m_Window);
    SetWindowLong(nativeWindow, GWL_STYLE, 0x0000);*/

    GLFWimage images;
    int imageWidth, imageHeight, imageComp;
    stbi_set_flip_vertically_on_load(true);

    stbi_uc* data = stbi_load("assets/resource/DarkIcon.png", &imageWidth, &imageHeight, &imageComp, 0);
    images.width  = imageWidth;
    images.height = imageHeight;
    images.pixels = data;

    glfwSetWindowIcon(m_Window, 1, &images);

    static int xpos, ypos, height, width;
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    glfwGetMonitorWorkarea(monitor, &xpos, &ypos, &width, &height);

    m_Data.PosX = width / 2 - m_Data.Width / 2;
    m_Data.PosY = height / 2 - m_Data.Height / 2;

    glfwSetWindowPos(m_Window, m_Data.PosX, m_Data.PosY);
    //glfwMaximizeWindow(m_Window);

    m_Context = new OpenGLContext(m_Window);
    m_Context->Init();

    glfwSetWindowUserPointer(m_Window, &m_Data);
    SetVSync(true);

    // Set GLFW callbacks
    glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
      WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
      data.Width       = width;
      data.Height      = height;

      WindowResizeEvent event(width, height);
      data.EventCallback(event);
    });

    glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
      WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
      WindowCloseEvent event;
      data.EventCallback(event);
    });

    glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
      WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

      switch (action)
      {
      case GLFW_PRESS: {
        KeyPressedEvent event(key, 0);
        data.EventCallback(event);
        break;
      }
      case GLFW_RELEASE: {
        KeyReleasedEvent event(key);
        data.EventCallback(event);
        break;
      }
      case GLFW_REPEAT: {
        KeyPressedEvent event(key, 1);
        data.EventCallback(event);
        break;
      }
      }
    });

    glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode) {
      WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

      KeyTypedEvent event(keycode);
      data.EventCallback(event);
    });

    glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods) {
      WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

      switch (action)
      {
      case GLFW_PRESS: {
        MouseButtonPressedEvent event(button);
        data.EventCallback(event);
        break;
      }
      case GLFW_RELEASE: {
        MouseButtonReleasedEvent event(button);
        data.EventCallback(event);
        break;
      }
      }
    });

    glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset) {
      WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

      MouseScrolledEvent event((float)xOffset, (float)yOffset);
      data.EventCallback(event);
    });

    glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos) {
      WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

      MouseMovedEvent event((float)xPos, (float)yPos);
      data.EventCallback(event);
    });

    glfwSetDropCallback(m_Window, [](GLFWwindow* window, int number, const char** path) {
      WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

      DropFileEvent event(number, path);
      data.EventCallback(event);
    });

    //glfwSetTitlebarHitTestCallback(m_Window, [](GLFWwindow* window, int xpos, int ypos, int* hit) {
    //  DK_CORE_INFO("tEST");
    //});
  }

  void WindowsWindow::OnUpdate()
  {
    glfwPollEvents();
#ifdef DK_RENDER_VULKAN

#else
    m_Context->SwapBuffers();
#endif
  }

  void WindowsWindow::Shutdown()
  {
#ifdef DK_RENDER_VULKAN
    vkDestroyInstance(m_Instance, nullptr);
#endif // DK_RENDER_VULKAN

    glfwDestroyWindow(m_Window);
    glfwTerminate();
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

  void WindowsWindow::SetWindowAttrib()
  {

    glfwSetWindowAttrib(m_Window, GLFW_DECORATED, GLFW_FALSE);
    //glfwSetWindowAttrib(m_Window, GLFW_TITLEBAR, GLFW_FALSE);
    //
    //glfwSetWindowAttrib(m_Window, GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_FALSE);
    glfwMaximizeWindow(m_Window);
    //glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
    //glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_FALSE);
  }

  void WindowsWindow::SetMinimize()
  {
    glfwIconifyWindow(m_Window);
  }

  void WindowsWindow::SetMaximizeOrRestore()
  {
    //glfwMaximizeWindow(m_Window);
    glfwRestoreWindow(m_Window);
  }

} // namespace Dark
