#include "dkpch.h"

#include <Dark.h>

class ExampleLayer : public Dark::Layer
{
public:
  ExampleLayer() :Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f)
  {
	//矩形顶点数据
	float vertices[] = {
		 0.5f, 0.5f, 0.0f, 0.8f, 0.2f, 0.3f, 1.0f,  // 右上角
		0.5f, -0.5f, 0.0f, 0.3f, 0.4f, 0.7f, 1.0f,  // 右下角
		-0.5f, -0.5f, 0.0f, 0.5f, 0.8f, 0.2f, 1.0f, // 左下角
		-0.5f, 0.5f, 0.0f, 0.2f, 0.5f, 0.3f, 1.0f   // 左上角
	};
	//索引绘制
	uint32_t indices[] = { // 注意索引从0开始! 
		0, 1, 3, // 第一个三角形
		1, 2, 3  // 第二个三角形
	};

	m_VertexArray.reset(Dark::VertexArray::Create());

	std::shared_ptr<Dark::VertexBuffer> m_VertexBuffer;
	m_VertexBuffer.reset(Dark::VertexBuffer::Create(vertices, sizeof(vertices)));
	Dark::BufferLayout layout = {
	  { Dark::ShaderDataType::Float3, "a_Pos" },
	  { Dark::ShaderDataType::Float4, "a_Color" }
	};
	m_VertexBuffer->SetLayout(layout);
	m_VertexArray->AddVertexBuffer(m_VertexBuffer);

	std::shared_ptr<Dark::IndexBuffer> m_IndexBuffer;
	m_IndexBuffer.reset(Dark::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
	m_VertexArray->SetIndexBuffer(m_IndexBuffer);

	std::string vertexShaderSource = R"(
	  #version 330 core
	  layout(location = 0) in vec3 a_Pos;
	  layout(location = 1) in vec4 a_Color;

	  uniform mat4 u_ViewProjection;

	  out vec4 v_Color;

	  void main() 
	  {
		gl_Position = u_ViewProjection * vec4(a_Pos, 1.0);
		v_Color = a_Color;
	  }
	)";

	std::string fragmentShaderSource = R"(
	  #version 330 core
	  
	  in  vec4 v_Color;
	  out vec4 FragColor;

	  void main() 
	  {
		FragColor = v_Color;
	  }
	)";

	m_Shader.reset(new Dark::Shader(vertexShaderSource, fragmentShaderSource));
  }

  void OnUpdate() override
  {
	if (Dark::Input::IsKeyPressed(DK_KEY_LEFT))
	  m_CameraPosition.x -= m_CameaSpeed;
	if (Dark::Input::IsKeyPressed(DK_KEY_RIGHT))
	  m_CameraPosition.x += m_CameaSpeed;
	if (Dark::Input::IsKeyPressed(DK_KEY_DOWN))
	  m_CameraPosition.y -= m_CameaSpeed;
	if (Dark::Input::IsKeyPressed(DK_KEY_UP))
	  m_CameraPosition.y += m_CameaSpeed;

	Dark::RenderCommand::SetClearColor({ 0.1f, 0.2f, 0.2f, 1.0f });
	Dark::RenderCommand::Clear();

	m_Camera.SetPosition(m_CameraPosition);
	m_Camera.SetRotation(0.0f);

	// Begin Rendering
	{
	  Dark::Renderer::BeginScene(m_Camera);
	  Dark::Renderer::Submit(m_Shader, m_VertexArray);
	  Dark::Renderer::EndScene();
	}
  }

  void OnImGuiRender() override
  {

  }

  void OnEvent(Dark::Event& event) override
  {

  }
private:
  glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
  float m_CameaSpeed = 0.1f;

  std::shared_ptr<Dark::VertexArray> m_VertexArray;
  std::shared_ptr<Dark::Shader> m_Shader;

  Dark::OrthographicCamera m_Camera;
};

class SandBox :public Dark::Application
{
public:
  SandBox()
  {
	PushLayer(new ExampleLayer());
  }
  ~SandBox()
  {
	
  }
};

Dark::Application* Dark::CreateApplication()
{
  return new SandBox();
}
