/*************************************************
Copyright © 2020 - 2022 YX.All Rights Reserved

Date:2022-10-5 16:09
Description:Timer
**************************************************/
#pragma once
#include <chrono>
#include "Dark/Core/Log.h"

template <typename Fn>
class Timer
{
public:
  Timer(const std::string name, Fn&& func)
      : m_Name(name), m_Func(func), m_Stopped(false)
  {
    m_StartTimepoint = std::chrono::high_resolution_clock::now();
  }

  ~Timer()
  {
    if (!m_Stopped)
      Stop();

    DK_CORE_TRACE("Timer: {0}, {1}ms", m_Name.c_str(), m_Duration);
  }

  void Stop()
  {
    auto endTimepoint = std::chrono::high_resolution_clock::now();

    long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
    long long end   = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

    m_Stopped = true;

    m_Duration = (end - start) * 0.001f;

    m_Func({m_Name.c_str(), m_Duration});
  }

private:
  const std::string m_Name;
  float m_Duration;
  std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;
  bool m_Stopped;
  Fn m_Func;
};