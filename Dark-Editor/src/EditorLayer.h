#pragma once
#include <Dark.h>

namespace Dark {

  class EditorLayer : public Layer
  {
  public:
	EditorLayer();

	void OnUpdate(Timestep timestep) override;
	void OnEvent(Event& event) override;
	void OnImGuiRender() override;

	virtual ~EditorLayer() = default;
  private:
	Dark::Ref<Dark::Texture2D> m_Texture;
  };

}
