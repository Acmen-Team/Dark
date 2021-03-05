#include "dkpch.h"

#include "VertexArray.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Dark {

  VertexArray* VertexArray::Create()
  {
	switch (Renderer::GetAPI())
	{
	  case RendererAPI::API::None:  DK_CORE_ASSERT(false, "RendererAPI::None is not supported!"); return nullptr;
	  case RendererAPI::API::OpenGL: return new OpenGLVertexArray();
	}

	DK_CORE_ASSERT(false, "Unkonw RendererAPI!");
	return nullptr;
  }

}