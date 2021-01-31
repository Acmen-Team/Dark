#include <Dark.h>

class ExampleLayer : public Dark::Layer
{
public:
  ExampleLayer() :Layer("Example") {}

  void OnUpdate() override
  {
	DK_INFO("ExampleLayer::Update");
  }

  void OnEvent(Dark::Event& event) override
  {
	DK_TRACE("{0}", event);
  }
};

class SandBox :public Dark::Application
{
public:
  SandBox()
  {
	PushLayer(new ExampleLayer());
	PushOverlay(new Dark::ImGuiLayer());
  }
  ~SandBox()
  {
	
  }
};

Dark::Application* Dark::CreateApplication()
{
  return new SandBox();
}
