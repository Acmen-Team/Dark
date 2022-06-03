#pragma once

#include <Dark.h>

#include <filesystem>

namespace Dark {

  class ContentBrowserPanel
  {
  public:
    ContentBrowserPanel();

    void OnImGuiRender();

  private:
    std::filesystem::path m_CurrentDirectory;

    Ref<Texture> m_DirectoryIcon;
  };

}