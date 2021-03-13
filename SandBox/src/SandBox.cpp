#include "dkpch.h"

#include <Dark.h>

#include "Platform/OpenGL/OpenGLShader.h"

#include <imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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

	Dark::Ref<Dark::VertexBuffer> m_VertexBuffer;
	m_VertexBuffer.reset(Dark::VertexBuffer::Create(vertices, sizeof(vertices)));
	Dark::BufferLayout layout = {
	  { Dark::ShaderDataType::Float3, "a_Pos" },
	  { Dark::ShaderDataType::Float4, "a_Color" }
	};
	m_VertexBuffer->SetLayout(layout);
	m_VertexArray->AddVertexBuffer(m_VertexBuffer);

	Dark::Ref<Dark::IndexBuffer> m_IndexBuffer;
	m_IndexBuffer.reset(Dark::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
	m_VertexArray->SetIndexBuffer(m_IndexBuffer);

	std::string vertexShaderSource = R"(
	  #version 330 core
	  layout(location = 0) in vec3 a_Pos;
	  layout(location = 1) in vec4 a_Color;

	  uniform mat4 u_ViewProjection;
	  uniform mat4 u_Transform;

	  out vec4 v_Color;

	  void main() 
	  {
		gl_Position = u_ViewProjection * u_Transform * vec4(a_Pos, 1.0);
		v_Color = a_Color;
	  }
	)";

	std::string fragmentShaderSource = R"(
	  #version 330 core

	  uniform vec3 u_Color;	  

	  in  vec4 v_Color;
	  out vec4 FragColor;

	  void main() 
	  {
		FragColor = vec4(u_Color, 1.0f);
	  }
	)";

	m_Shader.reset(Dark::Shader::Create(vertexShaderSource, fragmentShaderSource));
  }

  void OnUpdate(Dark::Timestep timestep) override
  {
	//DK_TRACE("Delta Time: {0}s  {1}ms", timestep.GetSeconds(), timestep.GetMilliseconds());

	if (Dark::Input::IsKeyPressed(DK_KEY_LEFT))
	  m_CameraPosition.x -= m_CameaSpeed * timestep.GetSeconds();
	if (Dark::Input::IsKeyPressed(DK_KEY_RIGHT))
	  m_CameraPosition.x += m_CameaSpeed * timestep.GetSeconds();
	if (Dark::Input::IsKeyPressed(DK_KEY_DOWN))
	  m_CameraPosition.y -= m_CameaSpeed * timestep.GetSeconds();
	if (Dark::Input::IsKeyPressed(DK_KEY_UP))
	  m_CameraPosition.y += m_CameaSpeed * timestep.GetSeconds();

	if (Dark::Input::IsKeyPressed(DK_KEY_A))
	  m_SquarPosition.x -= m_CameaSpeed * timestep.GetSeconds();
	if (Dark::Input::IsKeyPressed(DK_KEY_D))
	  m_SquarPosition.x += m_CameaSpeed * timestep.GetSeconds();
	if (Dark::Input::IsKeyPressed(DK_KEY_S))
	  m_SquarPosition.y -= m_CameaSpeed * timestep.GetSeconds();
	if (Dark::Input::IsKeyPressed(DK_KEY_W))
	  m_SquarPosition.y += m_CameaSpeed * timestep.GetSeconds();

	Dark::RenderCommand::SetClearColor({ 0.1f, 0.2f, 0.2f, 1.0f });
	Dark::RenderCommand::Clear();

	m_Camera.SetPosition(m_CameraPosition);
	m_Camera.SetRotation(0.0f);

	std::dynamic_pointer_cast<Dark::OpenGLShader>(m_Shader)->use();
	std::dynamic_pointer_cast<Dark::OpenGLShader>(m_Shader)->UploadUniformFloat3("u_Color", m_SquareColor);

	glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_SquarPosition);

	// Begin Rendering
	{
	  Dark::Renderer::BeginScene(m_Camera);
	  Dark::Renderer::Submit(m_Shader, m_VertexArray, transform);
	  Dark::Renderer::Submit(m_Shader, m_VertexArray);
	  Dark::Renderer::EndScene();
	}
  }

  void OnImGuiRender() override
  {
	ImGui::Begin("Setting");
	ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
  }

  void OnEvent(Dark::Event& event) override
  {
  }
private:
  glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
  float m_CameaSpeed = 0.8f;

  glm::vec3 m_SquarPosition = { 0.0f, 0.0f, 0.0f };

  Dark::Ref<Dark::VertexArray> m_VertexArray;
  Dark::Ref<Dark::Shader> m_Shader;

  Dark::OrthographicCamera m_Camera;

  glm::vec3 m_SquareColor = { 0.1f, 0.5f, 0.3f };
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
