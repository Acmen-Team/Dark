#pragma once

#ifdef DK_PLATFORM_WINDOWS

extern Dark::Application* Dark::CreateApplication();

#ifdef DK_RELEASE
  #pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )
#endif // DK_DEBUG

int main()
{
  int mainVer = 0;
  int minVer = 3;

  Dark::Log::Init();

  DK_CORE_TRACE("------Initialized Log!------");
  DK_CORE_INFO("------Dark Engine StartUp!------");
  DK_CORE_INFO("------Version(Alpha):{0}.{1}-------", mainVer, minVer);
  DK_INFO("------SandBox Application Created!------");

  auto app = Dark::CreateApplication();
  app->StartUp();
  delete app;
}
#endif