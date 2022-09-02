#pragma once
#include <Dark.h>

namespace Dark {

  class StartLayer : public Layer
  {
  public:
    StartLayer();
    virtual ~StartLayer() = default;

    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void OnUpdate(Timestep timestep) override;
    virtual void OnEvent(Event& event) override;

    virtual void OnImGuiRender() override;

  protected:
    void Init();
  private:
    //glm::vec3 m_SquarPosition1 = {0.0f, 0.0f, 0.0f};
    //glm::vec3 m_SquarPosition2 = {0.0f, 0.0f, 0.0f};

    //Ref<VertexArray> m_VertexArray;

    Ref<Texture2D> m_EngineLogo;
    glm::vec2 posMin;
    glm::vec2 posMax;
    //OrthographicCameraController m_CameraController;

    //glm::vec4 m_SquareColor = {0.7f, 0.1f, 0.1f, 0.7f};

    //Ref<Framebuffer> m_Framebuffer;

    //glm::vec2 m_ViewportSize = {0.0f, 0.0f};
    std::future<void> m_InitFunc;
  };

} // namespace Dark
