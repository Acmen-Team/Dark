#include "dkpch.h"

#include "RendererAPI.h"

namespace Dark {

#ifdef DK_RENDER_VULKAN
  RendererAPI::API RendererAPI::s_API = RendererAPI::API::Vulkan;
#else
  RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;
#endif // DK_RENDER_VULKAN

} // namespace Dark