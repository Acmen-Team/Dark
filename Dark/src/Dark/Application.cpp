#include "Application.h"
#include "Events/ApplicationEvent.h"
#include "Log.h"


namespace Dark {
	Application::Application()
	{

	}

	Application::~Application()
	{

	}

	void Application::StartUp()
	{
	  WindowResizeEvent e(1280, 720);
	  if (e.IsInCategory(EventCategoryApplication))
	  {
		DK_TRACE(e);
	  }
	  if (e.IsInCategory(EventCategoryInput))
	  {
		DK_TRACE(e);
	  }

	  while (true);
	}
}