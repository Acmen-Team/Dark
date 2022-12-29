#include "dkpch.h"

#include "ToolBarPanel.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

namespace Dark {

  const float toolbarSize = 50;
  float menuBarHeight     = 0;

  void DockSpaceUI()
  {
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImVec2 pos              = ImVec2(viewport->Pos.x, viewport->Pos.y + toolbarSize);
    ImVec2 size             = ImVec2(viewport->Size.x, viewport->Size.y - toolbarSize);
    ImGui::SetNextWindowPos(pos);
    ImGui::SetNextWindowSize(size);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGuiWindowFlags window_flags = 0 | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::Begin("Master DockSpace", NULL, window_flags);
    ImGuiID dockMain = ImGui::GetID("MyDockspace");

    // Save off menu bar height for later.
    menuBarHeight = ImGui::GetCurrentWindow()->MenuBarHeight();

    ImGui::DockSpace(dockMain);
    ImGui::End();
    ImGui::PopStyleVar(3);
  }

  void ToolbarUI()
  {
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(viewport->Pos.x, viewport->Pos.y + menuBarHeight));
    ImGui::SetNextWindowSize(ImVec2(viewport->Size.x, toolbarSize));
    ImGui::SetNextWindowViewport(viewport->ID);

    ImGuiWindowFlags window_flags = 0 | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings;
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
    ImGui::Begin("TOOLBAR", NULL, window_flags);
    ImGui::PopStyleVar();

    ImGui::Button("Toolbar goes here", ImVec2(0, 37));

    ImGui::End();
  }

  ToolBarPanel::ToolBarPanel()
  {
    m_PlayIcon  = ResourceManager::Get().GetResourceAllocator()->GetResource<Texture>("assets/resource/play50.png");
    m_PauseIcon = ResourceManager::Get().GetResourceAllocator()->GetResource<Texture>("assets/resource/pause50.png");
    m_RunIcon   = m_PlayIcon;
  }

  void ToolBarPanel::OnImGuiRender()
  {
    //DockSpaceUI();
    //ToolbarUI();
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollWithMouse;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::Begin("ToolBar", 0, flags);
    ImGuiDockNode* node = ImGui::GetWindowDockNode();
    node->LocalFlags |= ImGuiDockNodeFlags_NoTabBar | ImGuiWindowFlags_NoDocking;
    //node->LocalFlags |= ImGuiDockNodeFlags_NoResizeY;
    //ImVec2 avail = ImGui::GetContentRegionAvail();
    //ImGui::SetNextWindowSize(ImVec2(avail.x, 10));

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.8f, 0.1f, 0.15f, 0.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.9f, 0.2f, 0.2f, 0.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.9f, 0.2f, 0.2f, 0.0f});
    ImVec2 curspos = ImGui::GetCursorPos();
    curspos.x += (ImGui::GetContentRegionAvailWidth() / 2 - m_PlayIcon->GetWidth() / 2);
    ImGui::SetCursorPos(curspos);
    if (ImGui::ImageButton((void*)m_RunIcon->GetRendererID(), ImVec2(m_PlayIcon->GetWidth(), m_PlayIcon->GetHeight()), ImVec2(0, 1), ImVec2(1, 0), 0.0f, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(1.0f, 1.0f, 1.0f, 0.7f)))
    {
      m_IsRuntime = !m_IsRuntime;

      if (m_IsRuntime)
      {
        m_RunIcon = m_PauseIcon;
      }
      else
      {
        m_RunIcon = m_PlayIcon;
      }
    }

    ImGui::PopStyleColor(3);
    ImGui::End();
    ImGui::PopStyleVar();
  }

} // namespace Dark