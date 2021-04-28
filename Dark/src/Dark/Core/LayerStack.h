/*************************************************
Copyright © 2020 - 2021 YX.All Rights Reserved

Date:2021-1-2 11:56
Description:LayerStack Class
**************************************************/
#pragma once

#include "Core.h"
#include "Layer.h"

#include <vector>

namespace Dark {

  class  DARK_API LayerStack
  {
  public:
	LayerStack();
	~LayerStack();

	void PushLayer(Layer* layer);
	void PopLayer(Layer* layer);
	void PushOverlay(Layer* overlay);
	void PopOverlay(Layer* overlay);

	std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
	std::vector<Layer*>::iterator end() { return m_Layers.end(); }
  private:
	std::vector<Layer*> m_Layers;
	unsigned int m_LayerInsertIndex = 0;
  };

}
