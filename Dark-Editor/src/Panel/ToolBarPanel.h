#pragma once

#include <Dark.h>

namespace Dark {

  class ToolBarPanel
  {
  public:
    ToolBarPanel();

    void OnImGuiRender();

    bool GetIsRuntime() { return m_IsRuntime; }

  protected:
  private:
    Ref<Texture> m_PlayIcon;
    Ref<Texture> m_PauseIcon;
    Ref<Texture> m_RunIcon;

    bool m_IsRuntime{false};
  };

} // namespace Dark