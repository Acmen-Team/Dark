/*************************************************
Copyright © 2020 - 2022 YX.All Rights Reserved

Date:2022-10-5 16:11
Description:Profiler
**************************************************/
#pragma once
#include "Dark/Core/Utility/Timer.h"

namespace Dark {

  struct ProfileResult
  {
    const char* Name;
    float Time;
  };

  class Profiler
  {
  public:

  private:
  };
  static std::vector<ProfileResult> m_ProfileResults;

#define PROFILE_SCOPE(name) Timer timer##_LINE_(name, [&](ProfileResult profileResult) { \
  m_ProfileResults.push_back(profileResult);                                             \
})
} // namespace Dark