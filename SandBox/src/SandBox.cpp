#include "dkpch.h"

#include <Dark.h>
#include <Dark/Core/EntryPoint.h>
#include "ImGui/imgui.h"

#include "Platform/OpenGL/OpenGLShader.h"

#include <imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ExampleLayer : public Dark::Layer
{
public:
  ExampleLayer()
      : Layer("Example"), m_CameraController(1280.0f / 720.0f)
  {
    //���ζ�������
    float vertices[] = {
        0.5f, 0.5f, 0.0f, 1.0f, 1.0f,   // ���Ͻ�
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  // ���½�
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // ���½�
        -0.5f, 0.5f, 0.0f, 0.0f, 1.0f   // ���Ͻ�
    };
    //��������ltr
    uint32_t indices[] = {
        // ע��������0��ʼ!
        0, 1, 3, // ��һ��������
        1, 2, 3  // �ڶ���������
    };

    m_VertexArray.reset(Dark::VertexArray::Create());

    Dark::Ref<Dark::VertexBuffer> m_VertexBuffer;
    m_VertexBuffer.reset(Dark::VertexBuffer::Create(vertices, sizeof(vertices)));
    Dark::BufferLayout layout = {
        {Dark::ShaderDataType::Float3, "a_Pos"},
        {Dark::ShaderDataType::Float2, "a_TexCoord"}};
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

    auto texShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");
    m_ShaderLibrary.Add(Dark::Shader::Create("ColorShader", vertexShaderSource, ColorfragmentShaderSource));

    m_Texture = Dark::ResourceManager::Get().GetResourceAllocator()->GetResource<Dark::Texture2D>("assets/textures/container.jpg");
    m_TextureBlend = Dark::ResourceManager::Get().GetResourceAllocator()->GetResource<Dark::Texture2D>("assets/textures/face.png");
    m_MouseCursor  = Dark::ResourceManager::Get().GetResourceAllocator()->GetResource<Dark::Texture2D>("assets/textures/Mouse.png");
    //m_PlayButton  = std::dynamic_pointer_cast<Dark::Texture2D> (m_PlayButton->LoadFromFile("assets/UI/Play.png").second);
    //m_Sprite      = m_Sprite->LoadFromFile("assets/UI/Sprite.png").second;
    //m_Emote       = m_Emote->LoadFromFile("assets/UI/Emote.png").second;
    m_EngineLogo  = Dark::ResourceManager::Get().GetResourceAllocator()->GetResource<Dark::Texture2D>("assets/textures/DarkEngine.png");

    std::dynamic_pointer_cast<Dark::OpenGLShader>(texShader)->use();
    std::dynamic_pointer_cast<Dark::OpenGLShader>(texShader)->UploadUniformInt("u_Texture", 0);

    //ImGui::SetMouseCursor(ImGuiMouseCursor_None);
  }

  void OnUpdate(Dark::Timestep timestep) override
  {

    //Update
    m_CameraController.OnUpdate(timestep);

    //DK_TRACE("Delta Time: {0}s  {1}ms", timestep.GetSeconds(), timestep.GetMilliseconds());

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

    Dark::RenderCommand::SetClearColor({0.0f, 0.0f, 0.0f, 1.0f});
    Dark::RenderCommand::Clear();

    auto texShader   = m_ShaderLibrary.Get("Texture");
    auto colorShader = m_ShaderLibrary.Get("ColorShader");
    std::dynamic_pointer_cast<Dark::OpenGLShader>(colorShader)->use();
    std::dynamic_pointer_cast<Dark::OpenGLShader>(colorShader)->UploadUniformFloat4("u_Color", m_SquareColor);

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
  }

  void OnImGuiRender() override
  {

    ImGui::SetMouseCursor(ImGuiMouseCursor_None);

    ImVec2 pos = ImGui::GetMousePos();

    ImVec2 windowPos = ImGui::GetMainViewport()->WorkPos;
    //DK_INFO("pos:{0},{1}", windowPos.x, windowPos.y);
    ImVec2 windowSize = ImGui::GetMainViewport()->WorkSize;
    //DK_INFO("size:{0},{1}", windowSize.x, windowSize.y);

    {
      ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoBackground;
      ImGui::Begin("Menu", NULL, flags);
      ImGui::SetWindowPos(windowPos);
      ImGui::SetWindowSize(windowSize);

      //auto posMinX  = windowPos.x + ((windowSize.x - m_PlayButton->GetWidth()) / 2);
      //auto posMinY  = windowPos.y + ((windowSize.y - m_PlayButton->GetHeight()) / 2);
      //auto posMaxX  = windowPos.x + ((windowSize.x + m_PlayButton->GetWidth()) / 2);
      //auto posMaxY  = windowPos.y + ((windowSize.y + m_PlayButton->GetHeight()) / 2);
      //ImVec2 posMin = ImVec2(posMinX, posMinY);
      //ImVec2 posMax = ImVec2(posMaxX, posMaxY);

      //ImGui::ImageButton((ImTextureID)m_PlayButton->GetRendererID(), ImVec2(m_PlayButton->GetWidth(), m_PlayButton->GetHeight()), ImVec2(0.0f, 0.5f), ImVec2(0.5f, 0.0f));
      //ImGui::ImageButton((ImTextureID)m_PlayButton->GetRendererID(), posMin, posMax, ImVec2{0.0f, 0.5f}, ImVec2{0.5f, 0.0f});

      //ImGui::GetForegroundDrawList()->AddImage((ImTextureID)m_PlayButton->GetRendererID(), posMin, posMax, ImVec2{0.0f, 0.5f}, ImVec2{0.5f, 0.0f});
      ImGui::End();
    }

    ImGui::GetForegroundDrawList()->AddImage((ImTextureID)m_MouseCursor->GetRendererID(), pos, ImVec2(pos.x + 16, pos.y + 16), ImVec2{0.0f, 1.0f}, ImVec2{1.0f, 0.0f});
    /*   ImGui::GetForegroundDrawList()->AddImage((ImTextureID)m_EngineLogo->GetRendererID(), ImVec2(100, 100), ImVec2(100 + m_EngineLogo->GetWidth(), 100 + m_EngineLogo->GetHeight()), ImVec2{0.0f, 1.0f}, ImVec2{1.0f, 0.0f});*/
  }

  void OnEvent(Dark::Event& event) override
  {
    m_CameraController.OnEvent(event);
  }

private:
  glm::vec3 m_SquarPosition1 = {0.0f, 0.0f, 0.0f};
  glm::vec3 m_SquarPosition2 = {0.0f, 0.0f, 0.0f};

  Dark::Ref<Dark::VertexArray> m_VertexArray;

  Dark::ShaderLibrary m_ShaderLibrary;
  Dark::Ref<Dark::Texture> m_Texture;
  Dark::Ref<Dark::Texture2D> m_TextureBlend;
  Dark::Ref<Dark::Texture2D> m_MouseCursor;
  Dark::Ref<Dark::Texture2D> m_EngineLogo;
  Dark::Ref<Dark::Texture2D> m_PlayButton;
  Dark::Ref<Dark::Texture2D> m_Sprite;
  Dark::Ref<Dark::Texture2D> m_Emote;

  Dark::OrthographicCameraController m_CameraController;

  glm::vec4 m_SquareColor = {0.7f, 0.1f, 0.1f, 0.7f};
};

class SandBox : public Dark::Application
{
public:
  SandBox()
      : Application("SandBox")
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
