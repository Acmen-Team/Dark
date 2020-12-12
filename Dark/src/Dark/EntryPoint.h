#pragma once

#ifdef DK_PLATFORM_WINDOWS

extern Dark::Application* Dark::CreateApplication();

int main()
{
	int mainVer = 1;
	int minVer = 0;

	Dark::Log::Init();

	DK_CORE_TRACE("------Initialized Log!------");
	DK_CORE_INFO("------Dark Engine StartUp!------");
	DK_CORE_INFO("------Version:{0}.{1}------", mainVer, minVer);
	DK_INFO("------SandBox Application Created!------");

	auto app = Dark::CreateApplication();
	app->StartUp();
	delete app;
}
#endif