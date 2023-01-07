#pragma once

#include "Dark/Renderer/RendererAPI.h"

namespace Dark {

  class OpenGLRendererAPI : public RendererAPI
  {
  public:
    OpenGLRendererAPI() {}
    ~OpenGLRendererAPI() = default;

    virtual void Init() override;
    virtual void SetViewport(float width, float height) override;
    virtual void SetClearColor(const glm::vec4& color) override;
    virtual void Clear() override;

    virtual void FlushAndFinish() override;
    virtual void PixelStore() override;
    virtual void ReadPixels(int x, int y, int width, int height, void* data) override;

    virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) override;
  };

} // namespace Dark