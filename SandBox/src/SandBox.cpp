#include "dkpch.h"

#include <Dark.h>
#include <Dark/Core/EntryPoint.h>

#include <imgui.h>

class ExampleLayer : public Dark::Layer
{
public:
  ExampleLayer()
      : Layer("Example")
  {
    DK_INFO("------Dark Demo Created!------");
    Dark::Application::Get().GetWindow().SetWindowAttrib();

    //ImGui::SetMouseCursor(ImGuiMouseCursor_None);
  }

  virtual void OnAttach() override
  {
    //m_PlayButton  = Dark::ResourceManager::Get().GetResourceAllocator()->GetResource<Dark::Texture>("assets/textures/Play.png");
    m_EngineLogo  = Dark::ResourceManager::Get().GetResourceAllocator()->GetResource<Dark::Texture>("assets/Resource/DarkEngine.png");
    m_MouseCursor = Dark::ResourceManager::Get().GetResourceAllocator()->GetResource<Dark::Texture>("assets/textures/Mouse.png");

    m_Serialize = Dark::CreateRef<Dark::Serialize>();

    m_Scene = m_Serialize->DeserializeRuntime("assets/scenes/SimpleScene.ds");

    m_Audio = Dark::CreateRef<Dark::Audio>();
    m_Audio->InitDevice();
    m_Audio->SetSound("assets/Audio/bgm.mp3");

    m_Steamworks = Dark::CreateRef<Dark::Steamworks>();
    m_Steamworks->InitSteamAPI();

    m_Audio->PlaySound();
  }

  virtual void OnUpdate(Dark::Timestep timestep) override
  {
    m_Scene->OnUpdateRunTime(timestep);
  }

  virtual void OnImGuiRender() override
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

      if (ImGui::Button("Exit"))
      {
        m_Audio->StopSound();
        m_Steamworks->ShutdownSteamAPI();
        Dark::Application::Get().Exit();
      }

      //ImGui::GetForegroundDrawList()->AddImage((ImTextureID)m_PlayButton->GetRendererID(), posMin, posMax, ImVec2{0.0f, 0.5f}, ImVec2{0.5f, 0.0f});
      ImGui::End();
    }
    // Powered By Dark Engine
    float EnginePosMinX = windowSize.x - 60;
    float EnginePosMinY = windowSize.y - 80;

    float EnginePosMaxX = windowSize.x - 10;
    float EnginePosMaxY = windowSize.y - 80 + (m_EngineLogo->GetHeight() * (50.0f / m_EngineLogo->GetWidth()));

    ImGui::GetForegroundDrawList()->AddImage((ImTextureID)m_EngineLogo->GetRendererID(), ImVec2(EnginePosMinX, EnginePosMinY), ImVec2(EnginePosMaxX, EnginePosMaxY), ImVec2{0.0f, 1.0f}, ImVec2{1.0f, 0.0f});

    ImGui::GetForegroundDrawList()->AddImage((ImTextureID)m_MouseCursor->GetRendererID(), pos, ImVec2(pos.x + 16, pos.y + 16), ImVec2{0.0f, 1.0f}, ImVec2{1.0f, 0.0f});
  }

  virtual void OnEvent(Dark::Event& event) override
  {
  }

private:
  Dark::Ref<Dark::Texture> m_MouseCursor;
  Dark::Ref<Dark::Texture> m_EngineLogo;
  //Dark::Ref<Dark::Texture> m_PlayButton;

  Dark::Ref<Dark::Scene> m_Scene;
  Dark::Ref<Dark::Serialize> m_Serialize;

  Dark::Ref<Dark::Audio> m_Audio;
  Dark::Ref<Dark::Steamworks> m_Steamworks;
};

class SandBox : public Dark::Application
{
public:
  SandBox()
      : Application("DarkDemo")
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
