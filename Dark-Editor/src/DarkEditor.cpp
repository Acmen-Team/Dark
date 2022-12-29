#include <Dark.h>
#include <Dark/Core/EntryPoint.h>

#include "StartLayer.h"
#include "EditorLayer.h"

namespace Dark {

  class DarkEditor : public Application
  {
  public:
    DarkEditor()
        : Application("Dark Editor")
    {
      //PushLayer(new EditorLayer());
      PushLayer(new StartLayer());
    }
    ~DarkEditor()
    {
    }
  };

  Application* CreateApplication()
  {
    return new DarkEditor();
  }

} // namespace Dark