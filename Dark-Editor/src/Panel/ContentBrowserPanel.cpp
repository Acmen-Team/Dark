#include "dkpch.h"

#include "Dark/Tools/String/StringTool.h"

#include "ContentBrowserPanel.h"

#include "Test.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <IconsFontAwesome6.h>

#include <string.h>

namespace Dark {

  extern const std::filesystem::path g_AssetPath = "assets";

  void ContentBrowserPanel::FolderTreeNode(std::filesystem::path& path, std::string& strs)
  {
    //ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_FramePadding;
    //ImGuiStyle& style        = ImGui::GetStyle();
    //ImGuiWindow* window      = ImGui::GetCurrentWindow();

    //const bool display_frame = (flags & ImGuiTreeNodeFlags_Framed) != 0;
    //const ImVec2 paddings    = (display_frame || (flags & ImGuiTreeNodeFlags_FramePadding)) ? style.FramePadding : ImVec2(style.FramePadding.x, ImMin(window->DC.CurrLineTextBaseOffset, style.FramePadding.y));

    //ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, paddings);

    if (ImGui::TreeNodeEx(path.string().c_str(), ImGuiTreeNodeFlags_FramePadding, strs.c_str()))
    {
      //path /= path.filename();
      for (auto& directoryEntry : std::filesystem::directory_iterator(path))
      {
        const auto& path = directoryEntry.path();
        if (directoryEntry.is_directory())
        {
          auto relativePath          = std::filesystem::relative(path, g_AssetPath);
          std::string filenameString = relativePath.filename().string();
          std::string strs           = std::string(ICON_FA_FOLDER) + " " + filenameString;

          //ImGui::GetStyle().IndentSpacing = 10;
          FolderTreeNode(const_cast<std::filesystem::path&>(path), strs);
          //ImGui::GetStyle().IndentSpacing = 30;
        }
        else
        {
          auto relativePath          = std::filesystem::relative(path, g_AssetPath);
          std::string filenameString = relativePath.filename().string();
          std::string strs           = std::string(ICON_FA_FILE) + " " + filenameString;

          ImGui::Text(strs.c_str());
        }
      }
      ImGui::TreePop();
      strs = std::string(ICON_FA_FOLDER) + " " + path.filename().string();
    }
    else
    {
      strs = std::string(ICON_FA_FOLDER) + " " + path.filename().string();
    }
    //ImGui::PopStyleVar();
  }

  void ContentBrowserPanel::ToolBarWindow()
  {
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.18f, 0.185f, 0.19f, 1.0f));
    ImGui::BeginChild("ToolBarWindow", ImVec2(0, 40), false);
    ImVec2 curCursorPos = ImGui::GetCursorPos();

    ImGui::SetWindowFontScale(1.1);
    static float y = ImGui::GetContentRegionAvail().y;

    ImVec2 textSize = ImGui::CalcTextSize("\xef\x81\xa7"
                                          " Add");
    textSize.x += 15;
    textSize.y += 10;

    float size  = textSize.y;
    float avail = ImGui::GetContentRegionAvail().y;

    float off = (avail - size) * 0.5;
    if (off > 0.0f)
      ImGui::SetCursorPosY(curCursorPos.y + off);

    ImGui::SetCursorPosX(curCursorPos.x + 7);

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.27f, 0.275f, 0.28f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.35f, 0.355f, 0.36f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.32f, 0.405f, 0.33f, 1.0f));
    ImGui::Button("\xef\x81\xa7"
                  " Add",
                  textSize);
    //ImGui::ButtonYxYEx(ImVec4(0.5, 0.6, 0.7, 1.0), textSize, ImGuiButtonFlags_None, "B\033[31m" ICON_FA_FOLDER "\033[mnsi ");
    ImGui::PopStyleColor(3);

    ImGui::SameLine(0, 10);
    textSize = ImGui::CalcTextSize(ICON_FA_DOWNLOAD " Import");
    textSize.x += 15;
    textSize.y += 10;
    ImGui::Button(ICON_FA_DOWNLOAD " Import", textSize);

    ImGui::SameLine(0, 10);
    textSize = ImGui::CalcTextSize("\xef\x83\x87"
                                   " Save All");
    textSize.x += 15;
    textSize.y += 10;
    ImGui::Button("\xef\x83\x87"
                  " Save All",
                  textSize);

    ImGui::SameLine(0, 10);

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.8f, 0.1f, 0.15f, 0.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.9f, 0.2f, 0.2f, 0.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.9f, 0.2f, 0.2f, 0.0f});

    off = (avail - m_LeftIcon->GetWidth()) * 0.5;
    if (off > 0.0f)
      ImGui::SetCursorPosY(curCursorPos.y + off);

    if (m_CurrentDirectory != std::filesystem::path(g_AssetPath))
    {
      if (ImGui::ImageButton((void*)m_LeftIcon->GetRendererID(), ImVec2(m_LeftIcon->GetWidth(), m_LeftIcon->GetHeight()), ImVec2(0, 1), ImVec2(1, 0), 0.0f, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(1.0f, 1.0f, 1.0f, 0.6f)))
      {
          m_CurrentDirectory = m_CurrentDirectory.parent_path();
      }
    }
    else
    {
      if (ImGui::ImageButton((void*)m_LeftIcon->GetRendererID(), ImVec2(m_LeftIcon->GetWidth(), m_LeftIcon->GetHeight()), ImVec2(0, 1), ImVec2(1, 0), 0.0f, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(1.0f, 1.0f, 1.0f, 0.2f)))
      {
      }
    }

    ImGui::SameLine(0, 10);

    if (ImGui::ImageButton((void*)m_RightIcon->GetRendererID(), ImVec2(m_RightIcon->GetWidth(), m_RightIcon->GetHeight()), ImVec2(0, 1), ImVec2(1, 0), 0.0f, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(1.0f, 1.0f, 1.0f, 0.6f)))
    {
    }

    ImGui::PopStyleColor(3);

    ImGui::SameLine(0, 10);

    off = (avail - size) * 0.5;
    if (off > 0.0f)
      ImGui::SetCursorPosY(curCursorPos.y + off);
    textSize = ImGui::CalcTextSize(ICON_FA_FOLDER
                                   " Content");
    textSize.x += 15;
    textSize.y += 10;

    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10);
    ImGui::Button(ICON_FA_FOLDER
                  " Content",
                  textSize);

    ImGui::PopStyleVar();

    ImGui::SameLine(0, 5);

    textSize = ImGui::CalcTextSize(ICON_FA_ANGLE_RIGHT);
    off      = (avail - textSize.y - ImGui::GetStyle().FramePadding.y) * 0.5;
    if (off > 0.0f)
      ImGui::SetCursorPosY(curCursorPos.y + off);
    ImGui::TextWrapped(ICON_FA_ANGLE_RIGHT);

    //if (m_CurrentDirectory.has_root_directory())
    auto& relativePath = std::filesystem::relative(m_CurrentDirectory, g_AssetPath);
    if (relativePath.compare(".") != 0)
    {
      auto& relativePathStr = relativePath.string();

      for (auto& iter : SplitString(relativePathStr, "\\"))
      {
        std::string directoryName = std::string(ICON_FA_FOLDER) + std::string(" ") + iter;

        ImGui::SameLine(0, 5);

        off = (avail - size) * 0.5;
        if (off > 0.0f)
          ImGui::SetCursorPosY(curCursorPos.y + off);
        textSize = ImGui::CalcTextSize(directoryName.c_str());
        textSize.x += 15;
        textSize.y += 10;

        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10);
        ImGui::Button(directoryName.c_str(),
                      textSize);

        ImGui::PopStyleVar();

        ImGui::SameLine(0, 5);

        textSize = ImGui::CalcTextSize(ICON_FA_ANGLE_RIGHT);
        off      = (avail - textSize.y - ImGui::GetStyle().FramePadding.y) * 0.5;
        if (off > 0.0f)
          ImGui::SetCursorPosY(curCursorPos.y + off);
        ImGui::TextWrapped(ICON_FA_ANGLE_RIGHT);
      }
      
    }

    //ImGui::SameLine(0, 5);
    //ImGui::TextAnsiColored(ImVec4(0.5, 0.6, 0.7, 1.0), "B\033[31m" ICON_FA_FOLDER "\033[mnsi ");

    ImGui::EndChild();
    ImGui::PopStyleColor();
  }

  void ContentBrowserPanel::FolderListWindow()
  {
    ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, 22);
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.2f, 0.205f, 0.21f, 1.0f));
    ImGui::BeginChild("FolderListWindow", ImVec2(w, 0), false, ImGuiWindowFlags_AlwaysUseWindowPadding);
    ImGui::SetWindowFontScale(1.3);

    for (auto& directoryEntry : std::filesystem::directory_iterator(g_AssetPath))
    {
      const auto& path           = directoryEntry.path();
      if(directoryEntry.is_directory())
      {
        auto relativePath          = std::filesystem::relative(path, g_AssetPath);
        std::string filenameString = relativePath.filename().string();
        std::string strs           = std::string(ICON_FA_FOLDER) + " " + filenameString;
        FolderTreeNode(const_cast<std::filesystem::path&>(path), strs);
      }
    }
    ImGui::EndChild();
    ImGui::PopStyleColor();
    ImGui::PopStyleVar();
  }

  void ContentBrowserPanel::ContentViewWindow()
  {
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.2f, 0.205f, 0.21f, 1.0f));
    ImGui::BeginChild("ContentViewWindow", ImVec2(0, 0), false);
    //if (m_CurrentDirectory != std::filesystem::path(g_AssetPath))
    //{
    //  if (ImGui::Button("<-"))
    //  {
    //    m_CurrentDirectory = m_CurrentDirectory.parent_path();
    //  }
    //}

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
        ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM_TEXTURE", strPath.c_str(), strPath.length());
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
    ImGui::EndChild();
    ImGui::PopStyleColor();
  }

  ContentBrowserPanel::ContentBrowserPanel()
      : m_CurrentDirectory(g_AssetPath)
  {
    m_DirectoryIcon = ResourceManager::Get().GetResourceAllocator()->GetResource<Texture>("assets/resource/folder.png");
    m_LeftIcon      = ResourceManager::Get().GetResourceAllocator()->GetResource<Texture>("assets/resource/left20.png");
    m_RightIcon     = ResourceManager::Get().GetResourceAllocator()->GetResource<Texture>("assets/resource/right20.png");
  }

  bool MyTreeNode(const char* label)
  {
    ImGuiContext& g     = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;

    ImU32 id   = window->GetID(label);
    ImVec2 pos = window->DC.CursorPos;
    ImRect bb(pos, ImVec2(pos.x + ImGui::GetContentRegionAvail().x, pos.y + g.FontSize + g.Style.FramePadding.y * 2));
    bool opened = ImGui::TreeNodeBehaviorIsOpen(id);
    bool hovered, held;
    if (ImGui::ButtonBehavior(bb, id, &hovered, &held, true))
      window->DC.StateStorage->SetInt(id, opened ? 0 : 1);
    if (hovered || held)
      window->DrawList->AddRectFilled(bb.Min, bb.Max, ImColor(held ? ImGuiCol_HeaderActive : ImGuiCol_HeaderHovered));

    // Icon, text
    float button_sz = g.FontSize + g.Style.FramePadding.y * 2;
    window->DrawList->AddRectFilled(pos, ImVec2(pos.x + button_sz, pos.y + button_sz), opened ? ImColor(255, 0, 0) : ImColor(0, 255, 0));
    ImGui::RenderText(ImVec2(pos.x + button_sz + g.Style.ItemInnerSpacing.x, pos.y + g.Style.FramePadding.y), label);

    ImGui::ItemSize(bb, g.Style.FramePadding.y);
    ImGui::ItemAdd(bb, id);

    if (opened)
      ImGui::TreePush(label);
    return opened;
  }

  void ContentBrowserPanel::OnImGuiRender()
  {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 3));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 3));
    //ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 3));
    ImGui::Begin(ICON_FA_FOLDER_CLOSED " Content Browser", 0, ImGuiWindowFlags_AlwaysUseWindowPadding);

    //ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10, 10));
    ToolBarWindow();
    //ImGui::PopStyleVar();

    FolderListWindow();

    ImGui::SameLine(0.0f, 1.0f);

    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.28f, 0.285f, 0.29f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.38f, 0.385f, 0.39f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.48f, 0.485f, 0.49f, 1.0f));
    ImGui::Button("##vsplitter", ImVec2(5.0f, -1));
    ImGui::PopStyleColor(3);
    if (ImGui::IsItemActive())
      w += ImGui::GetIO().MouseDelta.x;

    ImGui::SameLine(0.0f, 1.0f);

    ContentViewWindow();

    ImGui::End();
    ImGui::PopStyleVar(2);
  }

} // namespace Dark