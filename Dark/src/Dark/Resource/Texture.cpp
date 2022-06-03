#include "dkpch.h"

#include "Texture.h"
#include "Dark/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLTexture.h"

namespace Dark {

Ref<Texture2D> Texture2D::Create(const std::string &path)
{
  switch (Renderer::GetAPI()) {
    case RendererAPI::API::None:
      DK_CORE_ASSERT(false, "RendererAPI::None is not supported!");
      return nullptr;
    case RendererAPI::API::OpenGL:
      return std::make_shared<OpenGLTexture2D>(path);
  }

  DK_CORE_ASSERT(false, "Unkonw RendererAPI!");
  return nullptr;
}

} // namespace Dark