/*************************************************
Copyright © 2020 - 2021 YX.All Rights Reserved

Date:2021-1-2 11:43
Description:Layer abstract base class
**************************************************/
#pragma once

#include "Dark/Core.h"
#include "Dark/Events/Event.h"

namespace Dark {

  class DARK_API Layer
  {
  public:
	Layer(const std::string& name = "Layer");
	virtual ~Layer();

	virtual void OnAttach() {}
	virtual void OnDetach() {}
	virtual void OnUpdate() {}
	virtual void OnEvent(Event& event) {}

	inline const std::string& GetName() const { return m_DebugName; }
  protected:
	std::string m_DebugName;
  };

}