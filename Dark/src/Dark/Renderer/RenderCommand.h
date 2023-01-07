#pragma once
#include "RendererAPI.h"

namespace Dark {

  class RenderCommand
  {
  public:
    inline static void Init()
    {
      s_RendererAPI->Init();
    }

    inline static void SetViewport(float width, float height)
    {
      s_RendererAPI->SetViewport(width, height);
    }

    inline static void SetClearColor(const glm::vec4& color)
    {
      s_RendererAPI->SetClearColor(color);
    }
    inline static void Clear()
    {
      s_RendererAPI->Clear();
    }

    inline static void FlushAndFinish()
    {
      s_RendererAPI->FlushAndFinish();
    }
    inline static void PixelStore()
    {
      s_RendererAPI->PixelStore();
    }
    inline static void ReadPixels(int x, int y, int width, int height, void* data)
    {
      s_RendererAPI->ReadPixels(x, y, width, height, data);
    }

    inline static void DrawIndexed(const Ref<VertexArray>& VertexArray)
    {
      s_RendererAPI->DrawIndexed(VertexArray);
    }

  private:
    static RendererAPI* s_RendererAPI;
  };

} // namespace Dark