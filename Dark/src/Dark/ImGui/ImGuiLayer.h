/*************************************************
Copyright © 2020 - 2021 YX.All Rights Reserved

Date:2021-1-24 23:11
Description:ImGuiLayer
**************************************************/
#pragma once

#include "Dark/Layer.h"

namespace Dark {

  class DARK_API ImGuiLayer : public Layer
  {
  public:
	ImGuiLayer();
	~ImGuiLayer();

	void OnAttach();
	void OnDetach();
	void OnUpdate();
	void OnEvent(Event& event);
  protected:

  private:
	float m_Time = 0.0f;
  };

}