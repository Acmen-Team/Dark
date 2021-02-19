#include <Dark.h>
#include <Dark/EntryPoint.h>

#include "EditorLayer.h"

namespace Dark {

 // class Editor : public Layer
 // {
 // public:
	//Editor() :Layer("Example") {}

	//void OnUpdate() override
	//{
	//  if (Input::IsKeyPressed(DK_KEY_W))
	//	DK_TRACE("W keyboard is Pressed!");

	//  //DK_INFO("ExampleLayer::Update");
	//}

	//void OnEvent(Event& event) override
	//{
	//  DK_TRACE("{0}", event);
	//}

	//void OnImGuiRender() override
	//{
	//  // In 99% case you should be able to just call DockSpaceOverViewport() and ignore all the code below!
	//// In this specific demo, we are not using DockSpaceOverViewport() because:
	//// - we allow the host window to be floating/moveable instead of filling the viewport (when opt_fullscreen == false)
	//// - we allow the host window to have padding (when opt_padding == true)
	//// - we have a local menu bar in the host window (vs. you could use BeginMainMenuBar() + DockSpaceOverViewport() in your code!)
	//// TL;DR; this demo is more complicated than what you would normally use.
	//// If we removed all the options we are showcasing, this demo would become:
	////     void ShowExampleAppDockSpace()
	////     {
	////         ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
	////     }

	//  static bool opt_fullscreen = true;
	//  static bool opt_padding = false;
	//  static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
	//  static bool *p_open;

	//  // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	//  // because it would be confusing to have two docking targets within each others.
	//  ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	//  if (opt_fullscreen)
	//  {
	//	ImGuiViewport* viewport = ImGui::GetMainViewport();
	//	ImGui::SetNextWindowPos(viewport->GetWorkPos());
	//	ImGui::SetNextWindowSize(viewport->GetWorkSize());
	//	ImGui::SetNextWindowViewport(viewport->ID);
	//	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	//	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	//	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	//	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	//  }
	//  else
	//  {
	//	dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
	//  }

	//  // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
	//  // and handle the pass-thru hole, so we ask Begin() to not render a background.
	//  if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
	//	window_flags |= ImGuiWindowFlags_NoBackground;

	//  // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
	//  // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
	//  // all active windows docked into it will lose their parent and become undocked.
	//  // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
	//  // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
	//  if (!opt_padding)
	//	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	//  ImGui::Begin("DockSpace Demo", p_open, window_flags);
	//  if (!opt_padding)
	//	ImGui::PopStyleVar();

	//  if (opt_fullscreen)
	//	ImGui::PopStyleVar(2);

	//  // DockSpace
	//  ImGuiIO& io = ImGui::GetIO();
	//  if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	//  {
	//	ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
	//	ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	//  }

	//  if (ImGui::BeginMenuBar())
	//  {
	//	if (ImGui::BeginMenu("File"))
	//	{
	//	  // Disabling fullscreen would allow the window to be moved to the front of other windows,
	//	  // which we can't undo at the moment without finer window depth/z control.
	//	  if (ImGui::MenuItem("Exit"))
	//		Dark::Application::Get().Exit();

	//	  ImGui::EndMenu();
	//	}

	//	ImGui::EndMenuBar();
	//  }

	//  static bool show_demo_window = true;
	//  static bool show_another_window = true;
	//  ImGui::ShowDemoWindow(&show_demo_window);
	//  //ImGui::ShowMetricsWindow(&show_demo_window);

	//  ImGui::Begin("Debug");

	//  float color[3] = { 0.3f, 0.2f, 0.5f };

	//  float f = 0.4f;
	//  ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
	//  ImGui::ColorEdit3("clear color", (float*)color); // Edit 3 floats representing a color

	//  unsigned int counter = 0;
	//  if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
	//	counter++;
	//  ImGui::SameLine();
	//  ImGui::Text("counter = %d", counter);

	//  ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	//  ImGui::End();

	//  ImGui::End();
	//}

	//virtual ~Editor() = default;
 // };

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