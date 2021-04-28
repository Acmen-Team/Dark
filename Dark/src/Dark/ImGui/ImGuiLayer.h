/*************************************************
Copyright © 2020 - 2021 YX.All Rights Reserved

Date:2021-1-24 23:11
Description:ImGuiLayer
**************************************************/
#pragma once

#include "Dark/Core/Layer.h"

#include "Dark/Events/ApplicationEvent.h"
#include "Dark/Events/KeyEvent.h"
#include "Dark/Events/MouseEvent.h"

namespace Dark {

  class DARK_API ImGuiLayer : public Layer
  {
  public:
	ImGuiLayer();
	~ImGuiLayer();

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnImGuiRender() override;

	void Begin();
	void End();
  private:
	float m_Time = 0.0f;
  };

}