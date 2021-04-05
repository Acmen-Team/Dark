#include "dkpch.h"

#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace Dark {

  static GLenum ShaderDataTypeToOPenGLBaseType(ShaderDataType type)
  {
	switch (type)
	{
	case Dark::ShaderDataType::Float:	  return GL_FLOAT;
	case Dark::ShaderDataType::Float2:  return GL_FLOAT;
	case Dark::ShaderDataType::Float3:  return GL_FLOAT;
	case Dark::ShaderDataType::Float4:  return GL_FLOAT;
	case Dark::ShaderDataType::Mat3:	  return GL_FLOAT;
	case Dark::ShaderDataType::Mat4:	  return GL_FLOAT;
	case Dark::ShaderDataType::Int:	  return GL_INT;
	case Dark::ShaderDataType::Int2:	  return GL_INT;
	case Dark::ShaderDataType::Int3:	  return GL_INT;
	case Dark::ShaderDataType::Int4:	  return GL_INT;
	case Dark::ShaderDataType::Bool:	  return GL_BOOL;
	}

	DK_CORE_ASSERT(false, "Unknown ShaderDataType!");
	return 0;
  }

  OpenGLVertexArray::OpenGLVertexArray()
  {
	glCreateVertexArrays(1, &m_RendererID);
  }

  OpenGLVertexArray::~OpenGLVertexArray()
  {
	glDeleteVertexArrays(1, &m_RendererID);
  }

  void OpenGLVertexArray::Bind() const
  {
	glBindVertexArray(m_RendererID);
  }

  void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
  {
	DK_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");

	glBindVertexArray(m_RendererID);
	vertexBuffer->Bind();

	uint32_t index = 0;
	const auto& layout = vertexBuffer->GetLayout();
	for (const auto& element : layout)
	{
	  glEnableVertexAttribArray(index);
	  glVertexAttribPointer(
		index,
		element.GetComponentCount(),
		ShaderDataTypeToOPenGLBaseType(element.Type),
		element.Normalized ? GL_TRUE : GL_FALSE,
		layout.GetStride(),
		(const void*)element.Offset
	  );
	  index++;
	}

	m_VertexBuffers.push_back(vertexBuffer);
  }

  void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
  {
	glBindVertexArray(m_RendererID);
	indexBuffer->Bind();

	m_IndexBuffer = indexBuffer;
  }

}