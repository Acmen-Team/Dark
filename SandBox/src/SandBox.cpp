#include <Dark.h>
#include <Dark/EntryPoint.h>
#include "ImGui/imgui.h"


class ExampleLayer : public Dark::Layer
{
public:
  ExampleLayer() :Layer("Example") {}

  void OnUpdate() override
  {
	if (Dark::Input::IsKeyPressed(DK_KEY_W))
	  DK_TRACE("W keyboard is Pressed!");

	//DK_INFO("ExampleLayer::Update");
  }

  void OnEvent(Dark::Event& event) override
  {
	DK_TRACE("{0}", event);
  }

  void OnImGuiRender() override
  {
	static bool show_demo_window = true;
	static bool show_another_window = true;
	ImGui::ShowDemoWindow(&show_demo_window);
	//ImGui::ShowMetricsWindow(&show_demo_window);

	ImGui::Begin("Debug");

	float color[3] = { 0.3f, 0.2f, 0.5f };

	float f = 0.4f;
	ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
	ImGui::ColorEdit3("clear color", (float*)color); // Edit 3 floats representing a color

	unsigned int counter = 0;
	if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
	  counter++;
	ImGui::SameLine();
	ImGui::Text("counter = %d", counter);

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();

	ImGui::End();
  }

  virtual ~ExampleLayer() = default;
};

class SandBox :public Dark::Application
{
public:
  SandBox() :Application("SandBox")
  {
	PushLayer(new ExampleLayer());
  }
  ~SandBox()
  {
	
  }
};

Dark::Application* Dark::CreateApplication()
{
  return new SandBox();
}
