#pragma once
#include "Panel/ContentBrowserPanel.h"
#include "Panel/SceneHierarchyPanel.h"
#include "Panel/ToolBarPanel.h"
#include "Panel/ECSPanel.h"

#include <Dark.h>

#include "EditorCameraController.h"

namespace Dark {

  class EditorLayer : public Layer
  {
  public:
    EditorLayer();
    virtual ~EditorLayer() = default;

    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void OnUpdate(Timestep timestep) override;
    virtual void OnEvent(Event& event) override;

    virtual void OnImGuiRender() override;

  private:
    // Editor Panel
    ECSPanel m_ECSPanel;
    ToolBarPanel m_ToolBar;
    ContentBrowserPanel m_Content;
    SceneHierarchyPanel m_SceneHierarchy;
  private:
    EditorCameraController m_CameraController;

    Entity m_SelectedEntity;
    // Scene
    Ref<Scene> m_Scene;
    Ref<Serialize> m_Serialize;

    Ref<Texture2D> m_EngineLogo;
    Ref<Texture2D> m_Minimize;
    Ref<Texture2D> m_Restore;
    Ref<Texture2D> m_Close;

    glm::vec4 m_SquareColor = {0.7f, 0.1f, 0.1f, 0.7f};

    Ref<Camera> m_MainCamera;
    Ref<Framebuffer> m_EditorViewFrameBuffer;
    Ref<Framebuffer> m_MainCameraFrameBuffer;

    glm::vec2 m_ViewportSize = {0.0f, 0.0f};

    bool m_ViewPanelFocused;
    bool m_ViewPanelHovered;
  };

} // namespace Dark
