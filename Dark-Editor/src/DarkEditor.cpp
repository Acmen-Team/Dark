#include <Dark.h>
#include <Dark/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace Dark {

  class DarkEditor :public Application
  {
  public:
	DarkEditor() :Application("Dark Editor")
	{
	  PushLayer(new EditorLayer());
	}
	~DarkEditor()
	{

	}
  };

  Application* CreateApplication()
  {
	return new DarkEditor();
  }

}