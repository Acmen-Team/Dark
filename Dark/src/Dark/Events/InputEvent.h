/*************************************************
Copyright © 2020 - 2022 YX.All Rights Reserved

Date:2022-7-10 16:32
Description:InputEvent
**************************************************/

#pragma once

#include "Event.h"

namespace Dark {

  class DARK_API DropFileEvent : public Event
  {
  public:
    DropFileEvent(int number, const char** path)
        : m_Number(number), m_Path(path) {}

    inline int GetFileNumber() const { return m_Number; }

    inline const char** GetFilePathList() const { return m_Path; }

    std::string ToString() const override
    {
      std::stringstream ss;
      ss << "Drop" << m_Number << " File\n";
      for (int i = 0; i < m_Number; i++)
        ss << "File(" << i << "):" << m_Path[i] << "\n";
      return ss.str();
    }

    EVENT_CLASS_TYPE(DropFile)
    EVENT_CLASS_CATEGORY(EventCategoryInput)
  private:
    int m_Number;
    const char** m_Path;
  };

} // namespace Dark