#pragma once

#include <Dark.h>

namespace Dark {

  class ContentBrowserPanel
  {
  public:
    ContentBrowserPanel();

    void OnImGuiRender();

  protected:
    void FolderTreeNode(std::filesystem::path& path, std::string& strs);
    void ToolBarWindow();
    void FolderListWindow();
    void ContentViewWindow();

  private:
    float w{200.0f};
    float h{300.0f};

    std::filesystem::path m_CurrentDirectory;
    std::filesystem::path m_CurrentFolder;

    Ref<Texture> m_DirectoryIcon;
    Ref<Texture> m_LeftIcon;
    Ref<Texture> m_RightIcon;
  };

} // namespace Dark