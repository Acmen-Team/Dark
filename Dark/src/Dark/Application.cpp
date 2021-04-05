#include "dkpch.h"
#include "Application.h"
#include "Log.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Dark {

  #define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

  Application* Application::m_Instance = nullptr;

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

  Application::Application()
  Application::Application(const std::string& name)
  {
	DK_CORE_ASSERT(!m_Instance, "Application already exists!");
	Application::m_Instance = this;

	m_Window = std::unique_ptr<Window>(Window::Create(WindowProps(name)));
	m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

	m_ImGuiLayer = new ImGuiLayer();
	PushOverlay(m_ImGuiLayer);

	//���ζ�������
	float vertices[] = {
		 0.5f, 0.5f, 0.0f, 0.8f, 0.2f, 0.3f, 1.0f,  // ���Ͻ�
		0.5f, -0.5f, 0.0f, 0.3f, 0.4f, 0.7f, 1.0f,  // ���½�
		-0.5f, -0.5f, 0.0f, 0.5f, 0.8f, 0.2f, 1.0f, // ���½�
		-0.5f, 0.5f, 0.0f, 0.2f, 0.5f, 0.3f, 1.0f   // ���Ͻ�
	};
	//��������
	uint32_t indices[] = { // ע��������0��ʼ! 
		0, 1, 3, // ��һ��������
		1, 2, 3  // �ڶ���������
	};

	//�����������VAO
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

	BufferLayout layout = {
	  { ShaderDataType::Float3, "a_Pos" },
	  { ShaderDataType::Float4, "a_Color" }
	};

	uint32_t index = 0;
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

	m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

	std::string vertexShaderSource = R"(
	  #version 330 core
	  layout(location = 0) in vec3 a_Pos;
	  layout(location = 1) in vec4 a_Color;

	  out vec4 v_Color;

	  void main() 
	  {
		gl_Position = vec4(a_Pos.x, a_Pos.y, a_Pos.z, 1.0);
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

	m_Shader.reset(new Shader(vertexShaderSource, fragmentShaderSource));
  }
  
  Application::~Application()
  {
  
  }
  
  void Application::StartUp()
  {
	WindowResizeEvent e(1280, 720);
	if (e.IsInCategory(EventCategoryApplication))
	{
	  while (m_Running)
	  {
		glClearColor(0.1f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		m_Shader->use();
		glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, 0);

		for (Layer* layer : m_LayerStack)
		  layer->OnUpdate();

		m_ImGuiLayer->Begin();
		for (Layer* layer : m_LayerStack)
		  layer->OnImGuiRender();
		m_ImGuiLayer->End();


		m_Window->OnUpdate();
	  }
	}
  }

  void Application::OnEvent(Event & e)
  {
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

	//DK_CORE_TRACE("{0}", e);

	for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
	{
	  (*--it)->OnEvent(e);
	  if(e.Handled)
		break;
	}
  }

  void Application::PushLayer(Layer* layer)
  {
	m_LayerStack.PushLayer(layer);
	layer->OnAttach();
  }

  void Application::PushOverlay(Layer* layer)
  {
	m_LayerStack.PushOverlay(layer);
	layer->OnAttach();
  }

  void Application::Exit()
  {
	m_Running = false;
  }

  bool Application::OnWindowClose(WindowCloseEvent & e)
  {
	m_Running = false;
    return true;
  }

}