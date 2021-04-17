#include "EditorLayer.h"
#include "ImGui/imgui.h"

#include "Platform/OpenGL/OpenGLShader.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Dark {

  EditorLayer::EditorLayer() :Layer("EditorLayer"), m_CameraController(1280.0f / 720.0f)
  {
	
  }

  void EditorLayer::OnAttach()
  {
	//矩形顶点数据
	float vertices[] = {
		 0.5f,  0.5f, 0.0f, 1.0f, 1.0f, // 右上角
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // 右下角
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // 左下角
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f   // 左上角
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
	m_ShaderLibrary.Add(Shader::Create("ColorShader", vertexShaderSource, ColorfragmentShaderSource));

	m_Texture = Texture2D::Create("assets/textures/container.jpg");
	m_TextureBlend = Texture2D::Create("assets/textures/face.png");
	m_DfaultTex = Texture2D::Create("assets/textures/Checkerboard.png");

	Dark::FramebufferSpecification fbSpec;
	fbSpec.Width = 1280;
	fbSpec.Height = 720;
	m_Framebuffer = Dark::Framebuffer::Create(fbSpec);

	std::dynamic_pointer_cast<OpenGLShader>(texShader)->use();
	std::dynamic_pointer_cast<OpenGLShader>(texShader)->UploadUniformInt("u_Texture", 0);
  }

  void EditorLayer::OnDetach()
  {

  }

  void EditorLayer::OnUpdate(Dark::Timestep timestep)
  {
	//DK_TRACE("Delta Time: {0}s  {1}ms", timestep.GetSeconds(), timestep.GetMilliseconds());
	m_CameraController.OnUpdate(timestep);

	if (Dark::Input::IsKeyPressed(DK_KEY_LEFT))
	  m_SquarPosition1.x -= 0.7f * timestep.GetSeconds();
	if (Dark::Input::IsKeyPressed(DK_KEY_RIGHT))
	  m_SquarPosition1.x += 0.7f * timestep.GetSeconds();
	if (Dark::Input::IsKeyPressed(DK_KEY_DOWN))
	  m_SquarPosition1.y -= 0.7f * timestep.GetSeconds();
	if (Dark::Input::IsKeyPressed(DK_KEY_UP))
	  m_SquarPosition1.y += 0.7f * timestep.GetSeconds();

	if (Dark::Input::IsKeyPressed(DK_KEY_I))
	  m_SquarPosition2.y += 0.7f * timestep.GetSeconds();
	if (Dark::Input::IsKeyPressed(DK_KEY_J))
	  m_SquarPosition2.x -= 0.7f * timestep.GetSeconds();
	if (Dark::Input::IsKeyPressed(DK_KEY_K))
	  m_SquarPosition2.y -= 0.7f * timestep.GetSeconds();
	if (Dark::Input::IsKeyPressed(DK_KEY_L))
	  m_SquarPosition2.x += 0.7f * timestep.GetSeconds();

	m_Framebuffer->Bind();

	Dark::RenderCommand::SetClearColor({ 0.1f, 0.2f, 0.2f, 1.0f });
	Dark::RenderCommand::Clear();

	auto texShader = m_ShaderLibrary.Get("Texture");
	auto colorShader = m_ShaderLibrary.Get("ColorShader");
	std::dynamic_pointer_cast<OpenGLShader>(colorShader)->use();
	std::dynamic_pointer_cast<OpenGLShader>(colorShader)->UploadUniformFloat4("u_Color", m_SquareColor);

	glm::mat4 transform1 = glm::translate(glm::mat4(1.0f), m_SquarPosition1);
	glm::mat4 transform2 = glm::translate(glm::mat4(1.0f), m_SquarPosition2) * glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 1.0f));


	// Begin Rendering
	{
	  Dark::Renderer::BeginScene(m_CameraController.GetCamera());

	  m_Texture->Bind();
	  Dark::Renderer::Submit(texShader, m_VertexArray, transform1);
	  m_TextureBlend->Bind();
	  Dark::Renderer::Submit(texShader, m_VertexArray);
	  Dark::Renderer::Submit(colorShader, m_VertexArray, transform2);
	  Dark::Renderer::Submit(colorShader, m_VertexArray, glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, -0.5f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 1.0f)));
	  Dark::Renderer::EndScene();
	}
	m_Framebuffer->UnBind();
  }

  void EditorLayer::OnEvent(Event& event)
  {
	//DK_TRACE("{0}", event);
	m_CameraController.OnEvent(event);
  }

  void EditorLayer::OnImGuiRender()
  {
	static bool opt_fullscreen = true;
	static bool opt_padding = false;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
	static bool *p_open;

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
	  ImGuiViewport* viewport = ImGui::GetMainViewport();
	  ImGui::SetNextWindowPos(viewport->GetWorkPos());
	  ImGui::SetNextWindowSize(viewport->GetWorkSize());
	  ImGui::SetNextWindowViewport(viewport->ID);
	  ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	  window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	  window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}
	else
	{
	  dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
	}

	// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
	// and handle the pass-thru hole, so we ask Begin() to not render a background.
	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
	  window_flags |= ImGuiWindowFlags_NoBackground;

	// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
	// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
	// all active windows docked into it will lose their parent and become undocked.
	// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
	// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
	if (!opt_padding)
	  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace Demo", p_open, window_flags);
	if (!opt_padding)
	  ImGui::PopStyleVar();

	if (opt_fullscreen)
	  ImGui::PopStyleVar(2);

	// DockSpace
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
	  ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
	  ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

	if (ImGui::BeginMenuBar())
	{
	  if (ImGui::BeginMenu("File"))
	  {
		// Disabling fullscreen would allow the window to be moved to the front of other windows,
		// which we can't undo at the moment without finer window depth/z control.
		if (ImGui::MenuItem("Exit"))
		  Dark::Application::Get().Exit();

		ImGui::EndMenu();
	  }

	  ImGui::EndMenuBar();
	}

	// Scene
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0.0, 0.0 });
	ImGui::Begin("Scene");
	ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
	if (m_ViewportSize != *((glm::vec2*)&viewportPanelSize))
	{
	  m_Framebuffer->Resize((uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y);
	  m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
	}
	ImGui::Image((void*)m_Framebuffer->GetColorAttachmentRendererID(), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0.0f, 1.0f }, ImVec2{ 1.0f, 0.0f });
	ImGui::End();
	ImGui::PopStyleVar();

	// Detail
	ImGui::Begin("Detail");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();

	// Texture
	ImGui::Begin("Texture");
	ImGui::Text("Default Texture");
	ImGui::ImageButton((void*)m_DfaultTex->GetRendererID(), ImVec2{ 64.0f, 64.0f });
	ImGui::End();

	//Setting
	ImGui::Begin("Setting");
	//Camera Rotation
	//ImGui::DragFloat3("Camera Rotation", glm::value_ptr(m_CameraRotation), 0.03f);
	ImGui::End();

	ImGui::End();
  }

}