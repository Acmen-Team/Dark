#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "Window.h"

namespace Dark {

	class DARK_API Application
	{
	public:
		Application();
		virtual ~Application();

		void StartUp();
	private:
	  std::unique_ptr<Window> m_Window;
	  bool m_Running = true;
	};

	// define in CLIENT
	Application* CreateApplication();
}

