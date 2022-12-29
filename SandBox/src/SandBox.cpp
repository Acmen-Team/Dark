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
    m_PlayButton  = Dark::ResourceManager::Get().GetResourceAllocator()->GetResource<Dark::Texture>("assets/textures/Play.png");
    m_MouseCursor = Dark::ResourceManager::Get().GetResourceAllocator()->GetResource<Dark::Texture>("assets/textures/Mouse.png");

    m_Serialize = Dark::CreateRef<Dark::Serialize>();

    m_Scene = m_Serialize->DeserializeRuntime("assets/scenes/SimpleScene.ds");
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

      auto posMinX  = windowPos.x + ((windowSize.x - m_PlayButton->GetWidth()) / 2);
      auto posMinY  = windowPos.y + ((windowSize.y - m_PlayButton->GetHeight()) / 2);
      auto posMaxX  = windowPos.x + ((windowSize.x + m_PlayButton->GetWidth()) / 2);
      auto posMaxY  = windowPos.y + ((windowSize.y + m_PlayButton->GetHeight()) / 2);
      ImVec2 posMin = ImVec2(posMinX, posMinY);
      ImVec2 posMax = ImVec2(posMaxX, posMaxY);

      //ImGui::ImageButton((ImTextureID)m_PlayButton->GetRendererID(), ImVec2(m_PlayButton->GetWidth(), m_PlayButton->GetHeight()), ImVec2(0.0f, 0.5f), ImVec2(0.5f, 0.0f));
      //ImGui::ImageButton((ImTextureID)m_PlayButton->GetRendererID(), posMin, posMax, ImVec2{0.0f, 0.5f}, ImVec2{0.5f, 0.0f});

      ImGui::GetForegroundDrawList()->AddImage((ImTextureID)m_PlayButton->GetRendererID(), posMin, posMax, ImVec2{0.0f, 0.5f}, ImVec2{0.5f, 0.0f});
      ImGui::End();
    }

    ImGui::GetForegroundDrawList()->AddImage((ImTextureID)m_MouseCursor->GetRendererID(), pos, ImVec2(pos.x + 16, pos.y + 16), ImVec2{0.0f, 1.0f}, ImVec2{1.0f, 0.0f});
    /*   ImGui::GetForegroundDrawList()->AddImage((ImTextureID)m_EngineLogo->GetRendererID(), ImVec2(100, 100), ImVec2(100 + m_EngineLogo->GetWidth(), 100 + m_EngineLogo->GetHeight()), ImVec2{0.0f, 1.0f}, ImVec2{1.0f, 0.0f});*/
  }

  virtual void OnEvent(Dark::Event& event) override
  {
  }

private:
  Dark::Ref<Dark::Texture> m_MouseCursor;
  Dark::Ref<Dark::Texture> m_EngineLogo;
  Dark::Ref<Dark::Texture> m_PlayButton;

  Dark::Ref<Dark::Scene> m_Scene;
  Dark::Ref<Dark::Serialize> m_Serialize;
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
