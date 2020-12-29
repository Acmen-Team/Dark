#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "Dark/Events/ApplicationEvent.h"

#include "Window.h"

namespace Dark {

	class DARK_API Application
	{
	public:
		Application();
		virtual ~Application();

		void StartUp();
		void OnEvent(Event& e);
	private:
	  bool OnWindowClose(WindowCloseEvent& e);

	  std::unique_ptr<Window> m_Window;
	  bool m_Running = true;
	};

	// define in CLIENT
	Application* CreateApplication();
}

