#pragma once
#include <Dark.h>

namespace Dark {

  class EditorLayer : public Layer
  {
  public:
	EditorLayer();

	void OnUpdate() override;
	void OnEvent(Event& event) override;
	void OnImGuiRender() override;

	virtual ~EditorLayer() = default;
  private:

  };

}
