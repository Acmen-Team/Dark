#include "dkpch.h"
#include "StartLayer.h"

#include "EditorLayer.h"

#include <imgui/imgui.h>

namespace Dark {

  StartLayer::StartLayer()
  {
  }

  void StartLayer::OnAttach()
  {
    m_EngineLogo = CreateRef<Texture2D>();
    m_EngineLogo = std::dynamic_pointer_cast<Texture2D>(m_EngineLogo->LoadFromFile("assets/Resource/DarkEngine.png").second);

    float windowPosX = Application::Get().GetWindow().GetPosX();
    float windowPosY = Application::Get().GetWindow().GetPosY();
    posMin           = glm::vec2(windowPosX, windowPosY);
    posMax           = glm::vec2(windowPosX + m_EngineLogo->GetWidth(), windowPosY + m_EngineLogo->GetHeight());

    //m_InitFunc = std::async(std::launch::async, &StartLayer::Init, this);
    Init();
  }

  void StartLayer::OnDetach()
  {
  }

  void StartLayer::OnUpdate(Timestep timestep)
  {
  }

  void StartLayer::OnEvent(Event& event)
  {
  }

  void StartLayer::OnImGuiRender()
  {
    ImGui::GetForegroundDrawList()->AddImage((ImTextureID)m_EngineLogo->GetRendererID(), ImVec2(posMin.x, posMin.y), ImVec2(posMax.x, posMax.y), ImVec2{0.0f, 1.0f}, ImVec2{1.0f, 0.0f});

    //Sleep(3000);
    //if (m_InitFunc._Is_ready())
    //{
    Application::Get().PushOverlay(new EditorLayer());
    //Application::Get().PopBottomLayer();
    //}
  }

  void StartLayer::Init()
  {
    ResourceManager::Get();
  }

} // namespace Dark