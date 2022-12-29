#include "dkpch.h"
#include "StartLayer.h"

#include "EditorLayer.h"

#include <imgui/imgui.h>

#include <IconsFontAwesome6.h>

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


    ImGuiIO& io = ImGui::GetIO();
        // Imgui Fonts
    io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/resource/fonts/Poppins/Poppins-Medium.ttf", 18.0f);
    //io.Fonts->AddFontFromFileTTF("assets/resource/fonts/Poppins/Poppins-Bold.ttf", 18.0f);
    //io.Fonts->AddFontFromFileTTF("assets/resource/fonts/Poppins/Poppins-Regular.ttf", 18.0f);
    //io.Fonts->AddFontFromFileTTF("assets/resource/fonts/Poppins/Poppins-Black.ttf", 18.0f);
    //io.Fonts->AddFontFromFileTTF("assets/resource/fonts/Poppins/Poppins-BlackItalic.ttf", 18.0f);
    //io.Fonts->AddFontFromFileTTF("assets/resource/fonts/Poppins/Poppins-ExtraLight.ttf", 18.0f);
    //io.Fonts->AddFontFromFileTTF("assets/resource/fonts/Poppins/Poppins-Italic.ttf", 18.0f);
    //io.Fonts->AddFontFromFileTTF("assets/resource/fonts/Poppins/Poppins-Light.ttf", 18.0f);
    //io.Fonts->AddFontFromFileTTF("assets/resource/fonts/Poppins/Poppins-Medium.ttf", 18.0f);
    //io.Fonts->AddFontFromFileTTF("assets/resource/fonts/Poppins/Poppins-ExtraBold.ttf", 18.0f);
    //io.Fonts->AddFontFromFileTTF("assets/resource/fonts/Poppins/Poppins-Regular.ttf", 18.0f);
    //io.Fonts->AddFontFromFileTTF("assets/resource/fonts/Poppins/Poppins-SemiBold.ttf", 18.0f);
    //io.Fonts->AddFontFromFileTTF("assets/resource/fonts/Poppins/Poppins-Thin.ttf", 18.0f);

    // merge in icons from Font Awesome
    static const ImWchar icons_ranges[] = {ICON_MIN_FA, ICON_MAX_16_FA, 0};
    ImFontConfig icons_config;
    icons_config.MergeMode  = true;
    icons_config.PixelSnapH = true;
    const float m_FontSize  = 18.0f;
    const float m_IconSize  = 20.0f;
    icons_config.GlyphOffset.y -= ((m_FontSize - m_IconSize));
    io.Fonts->AddFontFromFileTTF("assets/resource/fonts/fa-solid-900-v5.ttf", 20.0f, &icons_config, icons_ranges);
    io.Fonts->AddFontFromFileTTF("assets/resource/fonts/fa-regular-400.ttf", 20.0f, &icons_config, icons_ranges);
    io.Fonts->AddFontFromFileTTF("assets/resource/fonts/fa-brands-400.ttf", 20.0f, &icons_config, icons_ranges);
    //io.Fonts->AddFontFromFileTTF("assets/resource/fonts/MaterialIcons-Regular.ttf", 18.0f, &icons_config, icons_ranges);
    //m_InitFunc = std::async(std::launch::async, &StartLayer::Init, this);
    Init();

    Application::Get().PushOverlay(new EditorLayer());
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
    //ImGui::GetForegroundDrawList()->AddImage((ImTextureID)m_EngineLogo->GetRendererID(), ImVec2(posMin.x, posMin.y), ImVec2(posMax.x, posMax.y), ImVec2{0.0f, 1.0f}, ImVec2{1.0f, 0.0f});

    //Sleep(3000);
    //if (m_InitFunc._Is_ready())
    //{
    //Application::Get().PopBottomLayer();
    //}
  }

  void StartLayer::Init()
  {
    ResourceManager::Get();
  }

} // namespace Dark