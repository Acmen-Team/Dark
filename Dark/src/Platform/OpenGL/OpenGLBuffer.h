#pragma once

#include "Dark/Renderer/Buffer.h"

namespace Dark {

  class OpenGLVertexBuffer : public VertexBuffer
  {
  public:
	OpenGLVertexBuffer(float* vertices, uint32_t size);
	virtual ~OpenGLVertexBuffer();

	virtual void Bind() const override;
  private:
	uint32_t m_RendererID;
  };

  class OpenGLIndexBuffer : public IndexBuffer
  {
  public:
	OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
	virtual ~OpenGLIndexBuffer();

	virtual void Bind() const override;
	virtual uint32_t GetCount() const override { return m_Count; }
  private:
	uint32_t m_RendererID;
	uint32_t m_Count;
  };

}