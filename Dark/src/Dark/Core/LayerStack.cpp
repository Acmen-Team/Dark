﻿/*************************************************
Copyright © 2020 - 2021 YX.All Rights Reserved

Date:2021-1-2 12:05
Description:LayerStack
**************************************************/
#include "dkpch.h"
#include "LayerStack.h"

namespace Dark {

  LayerStack::LayerStack()
  {
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
    m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
    m_LayerInsertIndex++;
  }

  void LayerStack::PopLayer(Layer* layer)
  {
    auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
    if (it != m_Layers.end())
    {
      m_Layers.erase(it);
      m_LayerInsertIndex--;
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

  void LayerStack::PopTopLayer()
  {
  }

  void LayerStack::PopBottomLayer()
  {
    m_Layers.pop_back();
  }

} // namespace Dark