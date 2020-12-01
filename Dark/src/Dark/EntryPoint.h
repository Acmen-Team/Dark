#pragma once

#ifdef DK_PLATFORM_WINDOWS

extern Dark::Application* Dark::CreateApplication();

int main()
{
	auto app = Dark::CreateApplication();
	app->StartUp();
	delete app;
}
#endif