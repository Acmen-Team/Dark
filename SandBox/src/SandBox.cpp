#include "dkpch.h"

#include <Dark.h>
#include <Dark/Core/EntryPoint.h>

#include <imgui.h>
#include <iostream>

#include "TicTacToe.h"

class ExampleLayer : public Dark::Layer
{
public:
  ExampleLayer()
      : Layer("Example")
  {
    DK_INFO("------Dark Demo Created!------");
    Dark::Application::Get().GetWindow().SetWindowAttrib();

    m_Chess = TTT::TicTacToe();

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

    m_AudioS1 = Dark::CreateRef<Dark::Audio>();
    m_AudioS1->SetSound("assets/Audio/s1.mp3");

    m_ShengLiAudio = Dark::CreateRef<Dark::Audio>();
    m_ShengLiAudio->SetSound("assets/Audio/shengli.mp3");

    m_Steamworks = Dark::CreateRef<Dark::Steamworks>();
    m_Steamworks->InitSteamAPI();

    m_Audio->PlaySound();
  }

  virtual void OnUpdate(Dark::Timestep timestep) override
  {
    if (m_SceneMousePosX > 0.0f && m_SceneMousePosX < 1.0f && m_SceneMousePosY > 0.0f && m_SceneMousePosY < 1.0f)
    {
      m_Scene->OnUpdateRunTime(timestep, m_SceneMousePosX * windowSize.x, windowSize.y - m_SceneMousePosY * windowSize.y);
    }
    else
    {
      m_Scene->OnUpdateRunTime(timestep, 0.0f, 0.0f);
    }

    m_SceneMousePosX = 0.0f;
    m_SceneMousePosY = 0.0f;

    m_SelectEntity = m_Scene->GetSelectEntity();
	m_Steamworks->RunCallBacks();
  }

  virtual void OnEvent(Dark::Event& event) override
  {
    Dark::EventDispatcher dispatcher(event);
    dispatcher.Dispatch<Dark::MouseButtonPressedEvent>(DK_BIND_EVENT_FN(ExampleLayer::OnMouseButtonPressed));
    dispatcher.Dispatch<Dark::MouseButtonReleasedEvent>(DK_BIND_EVENT_FN(ExampleLayer::OnMouseButtonReleased));
  }

  virtual void OnImGuiRender() override
  {
    ImGui::SetMouseCursor(ImGuiMouseCursor_None);

    ImVec2 pos = ImGui::GetMousePos();

    windowPos = ImGui::GetMainViewport()->WorkPos;
    //DK_INFO("pos:{0},{1}", windowPos.x, windowPos.y);
    windowSize = ImGui::GetMainViewport()->WorkSize;
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

	  if (ImGui::Button("Create"))
	  {
		  m_Steamworks->CreatLobbys();
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

protected:
  bool OnMouseButtonPressed(Dark::MouseButtonPressedEvent& e)
  {
    if (e.GetMouseButton() == DK_MOUSE_BUTTON_LEFT)
    {
      windowPos = ImGui::GetMainViewport()->WorkPos;
      //DK_INFO("pos:{0},{1}", windowPos.x, windowPos.y);
      windowSize = ImGui::GetMainViewport()->WorkSize;

      m_SceneMousePosX = (ImGui::GetMousePos().x - windowPos.x) / windowSize.x;
      m_SceneMousePosY = (ImGui::GetMousePos().y - windowPos.y) / windowSize.y;

      DK_INFO("POS:{0}, {1}", m_SceneMousePosX, m_SceneMousePosY);
    }

    return false;
  }

  bool OnMouseButtonReleased(Dark::MouseButtonReleasedEvent& e)
  {
    m_SelectEntity = m_Scene->GetSelectEntity();

    if (m_SelectEntity != nullptr)
    {
      auto& tagName = m_SelectEntity->GetComponent<Dark::TagComponent>().Tag;
      DK_INFO("tag:{0}", tagName.c_str());

      // If a player wins, it is not allowed to continue
      if (m_Chess.GetWinFlag())
      {
        return false;
      }

      if (!m_SelectEntity->HasComponent<Dark::MaterialComponent>())
      {
        if (!m_Chess.PlayChess(tagName))
        {
          return false;
        }

        if (m_Chess.GetOffensive())
        {
          // True is turn to [xingxing] operator
          m_SelectEntity->AddComponent<Dark::MaterialComponent>(Dark::ResourceManager::Get().s_ShaderLibrary->Get("Texture"),
                                                                Dark::ResourceManager::Get().GetResourceAllocator()->GetResource<Dark::Texture>("assets/textures/yitiaoxin/xqizi.png"));
        }
        else
        {
          // False is turn to [yueliang] operator
          m_SelectEntity->AddComponent<Dark::MaterialComponent>(Dark::ResourceManager::Get().s_ShaderLibrary->Get("Texture"),
                                                                Dark::ResourceManager::Get().GetResourceAllocator()->GetResource<Dark::Texture>("assets/textures/yitiaoxin/yqizi.png"));
        }

        // Check Win
        if (m_Chess.CheckWin())
        {
          if (m_Chess.GetOffensive())
          {
            m_ShengLiAudio->PlaySound();
            DK_INFO("xingxing WIN");
          }
          else
          {
            m_ShengLiAudio->PlaySound();
            DK_INFO("yueliang WIN");
          }
        }
        // If no player wins, move on to the next player
        else
        {
          m_Chess.SetOffensive();
        }
      }
      else
      {
        if (!m_Chess.GetWinFlag())
        {
          DK_CORE_INFO("dogfall");
          return false;
        }
      }

      m_AudioS1->PlaySound();
    }

    return false;
  }

private:
  Dark::Ref<Dark::Texture> m_MouseCursor;
  Dark::Ref<Dark::Texture> m_EngineLogo;
  //Dark::Ref<Dark::Texture> m_PlayButton;

  Dark::Ref<Dark::Scene> m_Scene;
  Dark::Ref<Dark::Serialize> m_Serialize;
  Dark::Ref<Dark::Entity> m_SelectEntity;

  Dark::Ref<Dark::Audio> m_Audio;
  Dark::Ref<Dark::Audio> m_AudioS1;
  Dark::Ref<Dark::Audio> m_ShengLiAudio;
  Dark::Ref<Dark::Steamworks> m_Steamworks;

  ImVec2 windowPos{};
  ImVec2 windowSize{};
  float m_SceneMousePosX{-1.0f};
  float m_SceneMousePosY{-1.0f};

  TTT::TicTacToe m_Chess;
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
