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
	//���ζ�������
	float vertices[] = {
		 0.5f,  0.5f, 0.0f, 1.0f, 1.0f, // ���Ͻ�
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // ���½�
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // ���½�
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f   // ���Ͻ�
	};
	//��������
	uint32_t indices[] = { // ע��������0��ʼ! 
		0, 1, 3, // ��һ��������
		1, 2, 3  // �ڶ���������
	};

	m_VertexArray.reset(Dark::VertexArray::Create());

	Dark::Ref<Dark::VertexBuffer> m_VertexBuffer;
	m_VertexBuffer.reset(Dark::VertexBuffer::Create(vertices, sizeof(vertices)));
	Dark::BufferLayout layout = {
	  { Dark::ShaderDataType::Float3, "a_Pos" },
	  { Dark::ShaderDataType::Float2, "a_TexCoord" }
	};
	m_VertexBuffer->SetLayout(layout);
	m_VertexArray->AddVertexBuffer(m_VertexBuffer);

	Dark::Ref<Dark::IndexBuffer> m_IndexBuffer;
	m_IndexBuffer.reset(Dark::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
	m_VertexArray->SetIndexBuffer(m_IndexBuffer);

	std::string vertexShaderSource = R"(
	  #version 330 core
	  layout(location = 0) in vec3 a_Pos;
	  layout(location = 1) in vec2 a_TexCoord;

	  uniform mat4 u_ViewProjection;
	  uniform mat4 u_Transform;

	  out vec2 v_TexCoord;

	  void main() 
	  {
		gl_Position = u_ViewProjection * u_Transform * vec4(a_Pos, 1.0);
		v_TexCoord = a_TexCoord;
	  }
	)";

	std::string fragmentShaderSource = R"(
	  #version 330 core

	  uniform sampler2D u_Texture;

	  in vec2 v_TexCoord;
	  out vec4 FragColor;

	  void main() 
	  {
		FragColor = texture(u_Texture, v_TexCoord);
	  }
	)";

	std::string ColorfragmentShaderSource = R"(
	  #version 330 core

	  uniform vec4 u_Color;

	  in vec2 v_TexCoord;
	  out vec4 FragColor;

	  void main() 
	  {
		FragColor = u_Color;
	  }
	)";

	auto& texShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");
	m_ShaderLibrary.Add(Dark::Shader::Create("ColorShader", vertexShaderSource, ColorfragmentShaderSource));
	
	m_Texture = Dark::Texture2D::Create("assets/textures/container.jpg");
	m_TextureBlend = Dark::Texture2D::Create("assets/textures/face.png");

	std::dynamic_pointer_cast<Dark::OpenGLShader>(texShader)->use();
	std::dynamic_pointer_cast<Dark::OpenGLShader>(texShader)->UploadUniformInt("u_Texture", 0);
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
	  m_SquarPosition1.x -= m_CameaSpeed * timestep.GetSeconds();
	if (Dark::Input::IsKeyPressed(DK_KEY_D))
	  m_SquarPosition1.x += m_CameaSpeed * timestep.GetSeconds();
	if (Dark::Input::IsKeyPressed(DK_KEY_S))
	  m_SquarPosition1.y -= m_CameaSpeed * timestep.GetSeconds();
	if (Dark::Input::IsKeyPressed(DK_KEY_W))
	  m_SquarPosition1.y += m_CameaSpeed * timestep.GetSeconds();

	if (Dark::Input::IsKeyPressed(DK_KEY_I))
	  m_SquarPosition2.y += m_CameaSpeed * timestep.GetSeconds();
	if (Dark::Input::IsKeyPressed(DK_KEY_J))
	  m_SquarPosition2.x -= m_CameaSpeed * timestep.GetSeconds();
	if (Dark::Input::IsKeyPressed(DK_KEY_K))
	  m_SquarPosition2.y -= m_CameaSpeed * timestep.GetSeconds();
	if (Dark::Input::IsKeyPressed(DK_KEY_L))
	  m_SquarPosition2.x += m_CameaSpeed * timestep.GetSeconds();

	Dark::RenderCommand::SetClearColor({ 0.1f, 0.2f, 0.2f, 1.0f });
	Dark::RenderCommand::Clear();

	m_Camera.SetPosition(m_CameraPosition);
	m_Camera.SetRotation(0.0f);

	auto texShader = m_ShaderLibrary.Get("Texture");
	auto colorShader = m_ShaderLibrary.Get("ColorShader");
	std::dynamic_pointer_cast<Dark::OpenGLShader>(colorShader)->use();
	std::dynamic_pointer_cast<Dark::OpenGLShader>(colorShader)->UploadUniformFloat4("u_Color", m_SquareColor);

	glm::mat4 transform1 = glm::translate(glm::mat4(1.0f), m_SquarPosition1);
	glm::mat4 transform2 = glm::translate(glm::mat4(1.0f), m_SquarPosition2) * glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 1.0f));

	
	// Begin Rendering
	{
	  Dark::Renderer::BeginScene(m_Camera);

	  m_Texture->Bind();
	  Dark::Renderer::Submit(texShader, m_VertexArray, transform1);
	  m_TextureBlend->Bind();
	  Dark::Renderer::Submit(texShader, m_VertexArray);
	  Dark::Renderer::Submit(colorShader, m_VertexArray, transform2);
	  Dark::Renderer::Submit(colorShader, m_VertexArray, glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, -0.5f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 1.0f)));
	  Dark::Renderer::EndScene();
	}
  }

  void OnImGuiRender() override
  {
	ImGui::Begin("Setting");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
  }

  void OnEvent(Dark::Event& event) override
  {
  }
private:
  glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
  float m_CameaSpeed = 0.8f;

  glm::vec3 m_SquarPosition1 = { 0.0f, 0.0f, 0.0f };
  glm::vec3 m_SquarPosition2 = { 0.0f, 0.0f, 0.0f };

  Dark::Ref<Dark::VertexArray> m_VertexArray;

  Dark::ShaderLibrary m_ShaderLibrary;
  Dark::Ref<Dark::Texture2D> m_Texture;
  Dark::Ref<Dark::Texture2D> m_TextureBlend;

  Dark::OrthographicCamera m_Camera;

  glm::vec4 m_SquareColor = { 0.7f, 0.1f, 0.1f, 0.7f };
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
