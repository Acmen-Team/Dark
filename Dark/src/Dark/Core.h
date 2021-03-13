#pragma once

#include <memory>

#ifdef DK_PLATFORM_WINDOWS
  #if DK_DYNAMIC_LINK
	#ifdef DK_BUILD_DLL
	  #define DARK_API _declspec(dllexport)
	#else
	  #define DARK_API _declspec(dllimport)
	#endif
  #else
	#define DARK_API
  #endif
#else
  #error Dark Only Support Windows!
#endif

#ifdef DK_ENABLE_ASSERTS
  #define DK_ASSERT(x, ...) { if(!(x)) { DK_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
  #define DK_CORE_ASSERT(x, ...) { if(!(x)) { DK_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
  #define DK_ASSERT(x, ...)
  #define DK_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define DK_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace Dark {

  template<typename T>
  using Scope = std::unique_ptr<T>;

  template<typename T>
  using Ref = std::shared_ptr<T>;

}