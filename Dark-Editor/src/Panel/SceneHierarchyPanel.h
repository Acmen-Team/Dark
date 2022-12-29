/*************************************************
Copyright © 2020 - 2022 YX.All Rights Reserved

Date:2022-10-9 9:03
Description:SceneHierarchy Panel
**************************************************/
#pragma once

#include <Dark.h>

namespace Dark {

  class SceneHierarchyPanel
  {
  public:
    SceneHierarchyPanel() = default;

    SceneHierarchyPanel(const Ref<Scene>& context);

    void SetContext(const Ref<Scene>& context);

    void OnImGuiRender();

    Entity GetSelectEntity() { return m_SelectionContext; }

  protected:
    void DrawEntityNode(Entity entity);
    void DrawComponents(Entity entity);
  private:
    Ref<Scene> m_Context;
    Entity m_SelectionContext;
  };

} // namespace Dark
