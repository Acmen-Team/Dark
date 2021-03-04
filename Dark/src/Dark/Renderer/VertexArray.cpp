#include "dkpch.h"

#include "VertexArray.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Dark {

  VertexArray* VertexArray::Create()
  {
	switch (Renderer::GetAPI())
	{
	  case RendererAPI::None:  DK_CORE_ASSERT(false, "RendererAPI::None is not supported!"); return nullptr;
	  case RendererAPI::OpenGL: return new OpenGLVertexArray();
	}

	DK_CORE_ASSERT(false, "Unkonw RendererAPI!");
	return nullptr;
  }

}