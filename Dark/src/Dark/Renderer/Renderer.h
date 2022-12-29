#pragma once

#include "RenderCommand.h"
#include "Camera.h"
#include "Dark/Resource/Shader.h"

#include <glm/glm.hpp>

namespace Dark {

  class Renderer
  {
  public:
	static void Init();

	static void WindowResize(float width, float height);

	static void BeginScene(Camera& camera);
	static void EndScene();

	static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));

	inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
  private:
	struct SceneData
	{
	  glm::mat4 ViewProjectMatrix;
	};

	static SceneData* m_SceneData;
  };

}