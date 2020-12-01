#pragma once

#ifdef DK_PLATFORM_WINDOWS
	#ifdef DK_BUILD_DLL
		#define DARK_API _declspec(dllexport)
	#else
		#define DARK_API _declspec(dllimport)
	#endif
#else
	#error Dark Only Support Windows!
#endif