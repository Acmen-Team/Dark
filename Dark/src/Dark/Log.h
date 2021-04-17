/*************************************************
Copyright © 2020 - 2020 YX.All Rights Reserved

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

Date:2020-12-6 23:34
Description:Log System
**************************************************/
#pragma once

#include <memory>

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Dark {

  class DARK_API Log
  {
  public:
	static void Init();

	inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
	inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
  private:
	static std::shared_ptr<spdlog::logger> s_CoreLogger;
	static std::shared_ptr<spdlog::logger> s_ClientLogger;
  };

}

// Core log macros
#define DK_CORE_TRACE(...)		::Dark::Log::GetCoreLogger()->trace(__VA_ARGS__);
#define DK_CORE_INFO(...)		::Dark::Log::GetCoreLogger()->info(__VA_ARGS__);
#define DK_CORE_WARN(...)		::Dark::Log::GetCoreLogger()->warn(__VA_ARGS__);
#define DK_CORE_ERROR(...)		::Dark::Log::GetCoreLogger()->error(__VA_ARGS__);
#define DK_CORE_FATAL(...)		::Dark::Log::GetCoreLogger()->fatal(__VA_ARGS__);

// App log macros
#define DK_TRACE(...)			::Dark::Log::GetClientLogger()->trace(__VA_ARGS__);
#define DK_INFO(...)			::Dark::Log::GetClientLogger()->info(__VA_ARGS__);
#define DK_WARN(...)			::Dark::Log::GetClientLogger()->warn(__VA_ARGS__);
#define DK_ERROR(...)			::Dark::Log::GetClientLogger()->error(__VA_ARGS__);
#define DK_FATAL(...)			::Dark::Log::GetClientLogger()->fatal(__VA_ARGS__);
