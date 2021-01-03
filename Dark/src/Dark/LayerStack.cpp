/*************************************************
Copyright © 2020 - 2021 YX.All Rights Reserved

Date:2021-1-2 12:05
Description:LayerStack
**************************************************/
#include "dkpch.h"
#include "LayerStack.h"

namespace Dark {

  LayerStack::LayerStack()
  {
	m_LayerInsert = m_Layers.begin();
  }

  LayerStack::~LayerStack()
  {
	for (Layer* layer : m_Layers)
	{
	  delete layer;
	  layer = nullptr;
	}
  }

  void LayerStack::PushLayer(Layer* layer)
  {
	m_LayerInsert = m_Layers.emplace(m_LayerInsert, layer);
  }
  
  void LayerStack::PopLayer(Layer* layer)
  {
	auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
	if (it != m_Layers.end())
	{
	  m_Layers.erase(it);
	  m_LayerInsert--;
	}
  }

  void LayerStack::PushOverlay(Layer* overlay)
  {
	m_Layers.emplace_back(overlay);
  }

  void LayerStack::PopOverlay(Layer* overlay)
  {
	auto it = std::find(m_Layers.begin(), m_Layers.end(), overlay);
	if (it != m_Layers.end())
	  m_Layers.erase(it);
  }

}