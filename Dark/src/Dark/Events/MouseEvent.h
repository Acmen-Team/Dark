/*************************************************
Copyright © 2020 - 2020 YX.All Rights Reserved

Date:2020-12-26 2:15
Description:Mouse Event
**************************************************/
#pragma once

#include "Event.h"

namespace Dark {

  class DARK_API MouseMovedEvent :public Event
  {
  public:
	MouseMovedEvent(float x, float y) :m_MouseX(x), m_MouseY(y) {}

	inline float GetX() const { return m_MouseX; }
	inline float GetY() const { return m_MouseY; }

	std::string ToString() const override
	{
	  std::stringstream ss;
	  ss << "MouseMovedEvent: " << m_MouseX << ", " << m_MouseY;
	  return ss.str();
	}

	EVENT_CLASS_TYPE(MouseMoved)
	EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
  private:
	float m_MouseX, m_MouseY;
  };

  class DARK_API MouseScrolledEvent : public Event
  {
  public:
	MouseScrolledEvent(float xOffset, float yOffset) : m_XOffset(xOffset), m_YOffset(yOffset) {}

	inline float GetXOffset() const { return m_XOffset; }
	inline float GetYOffset() const { return m_YOffset; }

	std::string ToString() const override
	{
	  std::stringstream ss;
	  ss << "MouseScrolledEvent: " << GetXOffset() << ", " << GetYOffset();
	  return ss.str();
	}

	EVENT_CLASS_TYPE(MouseScrolled)
	EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
  private:
	float m_XOffset, m_YOffset;
  };

  class DARK_API MouseButtonEvent : public Event
  {
  public:
	inline int GetMouseButton() const { return m_Button; }

	EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
  protected:
	MouseButtonEvent(int button) : m_Button(button) {}

	int m_Button;
  };

  class DARK_API MouseButtonPressedEvent : public MouseButtonEvent
  {
  public:
	MouseButtonPressedEvent(int button) : MouseButtonEvent(button) {}

	std::string ToString() const override
	{
	  std::stringstream ss;
	  ss << "MouseButtonPressedEvent: " << m_Button;
	  return ss.str();
	}

	EVENT_CLASS_TYPE(MouseButtonPressed)
  };

  class DARK_API MouseButtonReleasedEvent : public MouseButtonEvent
  {
  public:
	MouseButtonReleasedEvent(int button) :MouseButtonEvent(button) {}

	std::string ToString() const override
	{
	  std::stringstream ss;
	  ss << "MouseButonReleasedEvent: " << m_Button;
	  return ss.str();
	}

	EVENT_CLASS_TYPE(MouseButtonReleased)
  };

}