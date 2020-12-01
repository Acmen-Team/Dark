#pragma once

#include "Core.h"

namespace Dark {

	class DARK_API Application
	{
	public:
		Application();
		virtual ~Application();

		void StartUp();
	};

	// define in CLIENT
	Application* CreateApplication();
}

