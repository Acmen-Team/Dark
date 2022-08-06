#pragma once

#include <Dark.h>

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

} // namespace Dark