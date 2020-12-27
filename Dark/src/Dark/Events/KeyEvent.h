/*************************************************
Copyright © 2020 - 2020 YX.All Rights Reserved

Date:2020-12-26 1:45
Description:Key Event
**************************************************/
#pragma once

#include "Event.h"

#include <sstream>

namespace Dark {

  class DARK_API KeyEvent :public Event
  {
  public:
	inline int GetKeyCode() const { return m_KeyCode; }

	EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
  protected:
	KeyEvent(int keycode) :m_KeyCode(keycode) {}

	int m_KeyCode;
  };

  class DARK_API KeyPressedEvent :public KeyEvent
  {
  public:
	KeyPressedEvent(int keycode, int repeatCount):KeyEvent(keycode), m_RepeatCount(repeatCount) {}

	inline int GetRepeatCount() const { return m_RepeatCount; }

	std::string ToString() const override
	{
	  std::stringstream ss;
	  ss << "KeyPressedEvent: " << m_KeyCode << " (" << m_RepeatCount << " repeats)";
	  return ss.str();
	}
  private:
	int m_RepeatCount;
  };

  class DARK_API KeyReleasedEvent :public KeyEvent
  {
  public:
	KeyReleasedEvent(int keycode) :KeyEvent(keycode) {}
	
	std::string ToString() const override
	{
	  std::stringstream ss;
	  ss << "KeyReleasedEvent: " << m_KeyCode;
	  return ss.str();
	}

	EVENT_CLASS_TYPE(keyReleased)
  };

}



