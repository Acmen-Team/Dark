/*************************************************
Copyright © 2020 - 2022 YX.All Rights Reserved

Date:2022-11-22 14:12
Description:Serialize
**************************************************/
#pragma once

#include "Dark/Scene/Scene.h"

namespace Dark {

  class Serialize
  {
  public:
    Serialize();
    Serialize(const Ref<Scene>& scene);

    void SerializeRuntime(const std::string& filePath);
    Ref<Scene> DeserializeRuntime(const std::string& filePath);

    void SetSerializeScene(Ref<Scene>& scene) { m_Scene = scene; }
  private:
    Ref<Scene> m_Scene;
    ShaderLibrary m_ShaderLibrary;
  };

}

