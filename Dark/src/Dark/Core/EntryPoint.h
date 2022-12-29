#pragma once

#ifdef DK_PLATFORM_WINDOWS

extern Dark::Application* Dark::CreateApplication();

#ifdef DK_RELEASE
  #pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )
#endif // DK_RELEASE

int main()
{
  int majorVer = 0;
  int minorVer = 1;
  int patchVer = 0;

  Dark::Log::Init();

  DK_CORE_TRACE("------------Initialized Log------------");
  DK_CORE_TRACE("------------Dark Engine StartUp------------");
  DK_CORE_INFO("------------Version(Alpha):{0}.{1}.{2}------------", majorVer, minorVer, patchVer);

  auto app = Dark::CreateApplication();
  app->StartUp();
  delete app;
}
#endif