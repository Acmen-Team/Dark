#include "dkpch.h"

#include "Buffer.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Dark {

  VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
  {
	switch (Renderer::GetAPI())
	{
	  case RendererAPI::None:  DK_CORE_ASSERT(false, "RendererAPI::None is not supported!"); return nullptr;
	  case RendererAPI::OpenGL: return new OpenGLVertexBuffer(vertices, size);
	}

	DK_CORE_ASSERT(false, "Unkonw RendererAPI!");
	return nullptr;
  }

  IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t count)
  {
	switch (Renderer::GetAPI())
	{
	case RendererAPI::None:  DK_CORE_ASSERT(false, "RendererAPI::None is not supported!"); return nullptr;
	case RendererAPI::OpenGL: return new OpenGLIndexBuffer(indices, count);
	}

	DK_CORE_ASSERT(false, "Unkonw RendererAPI!");
	return nullptr;
  }

}