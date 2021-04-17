#include "dkpch.h"

#include "Framebuffer.h"

#include "Dark/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace Dark {

  Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
  {
	switch (Renderer::GetAPI())
	{
	case RendererAPI::API::None:  DK_CORE_ASSERT(false, "RendererAPI::None is not supported!"); return nullptr;
	case RendererAPI::API::OpenGL: return std::make_shared<OpenGLFramebuffer>(spec);
	}

	DK_CORE_ASSERT(false, "Unkonw RendererAPI!");
	return nullptr;
  }

}