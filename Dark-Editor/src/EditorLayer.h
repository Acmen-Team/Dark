#pragma once
#include "Panel/ContentBrowserPanel.h"

#include <Dark.h>

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

  protected:
    
  private:
    glm::vec3 m_SquarPosition1 = {0.0f, 0.0f, 0.0f};
    glm::vec3 m_SquarPosition2 = {0.0f, 0.0f, 0.0f};

    Ref<VertexArray> m_VertexArray;

    ShaderLibrary m_ShaderLibrary;
    Ref<Texture2D> m_Texture;
    Ref<Texture2D> m_TextureBlend;
    Ref<Texture2D> m_DfaultTex;

    OrthographicCameraController m_CameraController;

    glm::vec4 m_SquareColor = {0.7f, 0.1f, 0.1f, 0.7f};

    Ref<Framebuffer> m_Framebuffer;

    glm::vec2 m_ViewportSize = {0.0f, 0.0f};

    ContentBrowserPanel m_Content;
  };

} // namespace Dark
