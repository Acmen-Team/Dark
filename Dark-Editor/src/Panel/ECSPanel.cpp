#include "dkpch.h"

#include "ECSPanel.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

namespace Dark {

  void ECSPanel::OnImGuiRender()
  {
    ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.1f, 0.105f, 0.11f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.1f, 0.105f, 0.11f, 1.0f));

    ImGuiID ecsDockSpace;
    ImGui::Begin("ECS");
    ImGui::Text("Search");
    ecsDockSpace = ImGui::GetID("EcsDockSpace");
     ImGui::DockSpace(ecsDockSpace);
    ImGui::End();

    ImGui::SetNextWindowDockID(ecsDockSpace);
    ImGui::Begin("Entity");
    ImGuiDockNode* node = ImGui::GetWindowDockNode();
    node->LocalFlags |= ImGuiDockNodeFlags_NoWindowMenuButton | ImGuiDockNodeFlags_NoCloseButton;

    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.2f, 0.205f, 0.21f, 1.0f));
    ImGui::BeginChild("##EntityPanel", ImVec2(0, 0));
    ImGui::Button("test");
    ImGui::Button("test");
    ImGui::Button("test");
    ImGui::Button("test");
    ImGui::EndChild();
    ImGui::PopStyleColor();

    ImGui::End();

    ImGui::SetNextWindowDockID(ecsDockSpace);
    ImGui::Begin("Component");
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.2f, 0.205f, 0.21f, 1.0f));
    ImGui::BeginChild("##EntityPanel", ImVec2(0, 0));
    ImGui::Button("test");
    ImGui::Button("test");
    ImGui::Button("test");
    ImGui::Button("test");
    ImGui::EndChild();
    ImGui::PopStyleColor();
    ImGui::End();

    ImGui::SetNextWindowDockID(ecsDockSpace);
    ImGui::Begin("System");
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.2f, 0.205f, 0.21f, 1.0f));
    ImGui::BeginChild("##EntityPanel", ImVec2(0, 0));
    ImGui::Button("test");
    ImGui::Button("test");
    ImGui::Button("test");
    ImGui::Button("test");
    ImGui::EndChild();
    ImGui::PopStyleColor();
    ImGui::End();

    ImGui::PopStyleColor(2);
  }

}