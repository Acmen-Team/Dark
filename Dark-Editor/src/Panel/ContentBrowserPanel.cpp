#include "dkpch.h"
#include "ContentBrowserPanel.h"

#include <imgui/imgui.h>

namespace Dark {

  extern const std::filesystem::path g_AssetPath = "assets";

  ContentBrowserPanel::ContentBrowserPanel()
      : m_CurrentDirectory(g_AssetPath)
  {
    m_DirectoryIcon = ResourceManager::Get().GetResourceAllocator()->GetResource<Texture2D>("assets/Resource/DirectoryIcon.png");
  }

  void ContentBrowserPanel::OnImGuiRender()
  {
    ImGui::Begin("Content Browser");

    if (m_CurrentDirectory != std::filesystem::path(g_AssetPath))
    {
      if (ImGui::Button("<-"))
      {
        m_CurrentDirectory = m_CurrentDirectory.parent_path();
      }
    }

    static float padding       = 16.0f;
    static float thumbnailSize = 64.0f;
    float cellSize             = thumbnailSize + padding;

    float panelWidth = ImGui::GetContentRegionAvail().x;
    int columnCount  = (int)(panelWidth / cellSize);
    if (columnCount < 1)
      columnCount = 1;

    ImGui::Columns(columnCount, 0, false);

    for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
    {
      const auto& path           = directoryEntry.path();
      auto relativePath          = std::filesystem::relative(path, g_AssetPath);
      std::string filenameString = relativePath.filename().string();

      Ref<Texture> icon = m_DirectoryIcon;

      ImGui::PushID(filenameString.c_str());

      std::string strPath = path.string();
      std::replace(strPath.begin(), strPath.end(), '\\', '/');
      if (std::string::npos != filenameString.find(".jpg") || std::string::npos != filenameString.find(".png"))
        icon = ResourceManager::Get().GetResourceAllocator()->GetResource<Texture2D>(strPath);

      ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
      ImGui::ImageButton((ImTextureID)icon->GetRendererID(), {thumbnailSize, thumbnailSize}, {0, 1}, {1, 0});

      if (ImGui::BeginDragDropSource())
      {
        const wchar_t* itemPath = relativePath.c_str();
        ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
        ImGui::EndDragDropSource();
      }

      ImGui::PopStyleColor();
      if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
      {
        if (directoryEntry.is_directory())
          m_CurrentDirectory /= path.filename();
      }
      ImGui::TextWrapped(filenameString.c_str());

      ImGui::NextColumn();

      ImGui::PopID();
    }

    ImGui::Columns(1);

    // ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 16, 512);
    // ImGui::SliderFloat("Padding", &padding, 0, 32);

    ImGui::End();
  }

} // namespace Dark