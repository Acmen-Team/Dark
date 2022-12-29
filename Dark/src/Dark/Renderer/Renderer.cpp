#include "dkpch.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Dark {

  Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;

  void Renderer::Init()
  {
	RenderCommand::Init();
  }

  void Renderer::WindowResize(float width, float height)
  {
	RenderCommand::SetViewport(width, height);
  }

  void Renderer::BeginScene(Camera& camera)
  {
	m_SceneData->ViewProjectMatrix = camera.GetViewProjectionMatrix();
  }

  void Renderer::EndScene()
  {
  }

  void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
  {
	shader->use();
	std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ViewProjection", m_SceneData->ViewProjectMatrix);
	std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Transform", transform);

	vertexArray->Bind();
	RenderCommand::DrawIndexed(vertexArray);
  }

}