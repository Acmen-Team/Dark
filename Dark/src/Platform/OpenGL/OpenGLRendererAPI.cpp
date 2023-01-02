#include "dkpch.h"

#include "OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace Dark {

  void OpenGLRendererAPI::Init()
  {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  }

  void OpenGLRendererAPI::SetViewport(float width, float height)
  {
	glViewport(0, 0, width, height);
  }

  void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
  {
	glClearColor(color.r, color.g, color.b, color.a);
  }

  void OpenGLRendererAPI::Clear()
  {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }

  void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray)
  {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

	glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
  }

}