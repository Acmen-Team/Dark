#pragma once
#include <Dark.h>

typedef void*(*DLLFUNC)(void*);

namespace Dark {

  class EditorLayer : public Layer
  {
  public:
	EditorLayer();
	virtual ~EditorLayer();

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(Timestep timestep) override;
	virtual void OnEvent(Event& event) override;

	virtual void OnImGuiRender() override;
  protected:
	HINSTANCE HDll;

	DLLFUNC dllfunc = nullptr;

	void* m_ContextData = nullptr;

	size_t ctxSize;

	void SelectEngineModule();
	void DarkEngine();
	void EngineModules();

	float alpha{ 0.1f };
  private:
	glm::vec3 m_SquarPosition1 = { 0.0f, 0.0f, 0.0f };
	glm::vec3 m_SquarPosition2 = { 0.0f, 0.0f, 0.0f };

	Ref<VertexArray> m_VertexArray;

	ShaderLibrary m_ShaderLibrary;
	Ref<Texture2D> m_Texture;
	Ref<Texture2D> m_TextureBlend;
	Ref<Texture2D> m_DfaultTex;
	Ref<Texture2D> m_DarkLogo;

	OrthographicCameraController m_CameraController;

	glm::vec4 m_SquareColor = { 0.7f, 0.1f, 0.1f, 0.7f };

	Ref<Framebuffer> m_Framebuffer;

	glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
  };

}
