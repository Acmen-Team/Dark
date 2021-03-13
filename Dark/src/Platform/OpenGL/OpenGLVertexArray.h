#pragma once

#include "Dark/Renderer/VertexArray.h"

namespace Dark {

  class OpenGLVertexArray : public VertexArray
  {
  public:
	OpenGLVertexArray();
	virtual ~OpenGLVertexArray();

	virtual void Bind() const override;

	virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) override;
	virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) override;

	virtual std::vector<Ref<VertexBuffer>> GetVertexBuffers() const override { return m_VertexBuffers; }
	virtual Ref<IndexBuffer> GetIndexBuffer() const override { return m_IndexBuffer; }
  private:
	uint32_t m_RendererID;
	std::vector<Ref<VertexBuffer>> m_VertexBuffers;
	Ref<IndexBuffer> m_IndexBuffer;
  };

}