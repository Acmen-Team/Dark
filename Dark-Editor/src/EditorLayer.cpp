#include "EditorLayer.h"

#include "Platform/OpenGL/OpenGLShader.h"

#include <imgui/imgui.h>
#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui_internal.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ImGuizmo.h>

#include <IconsFontAwesome6.h>

namespace Dark {

  EditorLayer::EditorLayer()
      : Layer("EditorLayer"), m_CameraController(1280.0f / 720.0f)
  {
    DK_INFO("------Dark Editor Created!------");
    Application::Get().GetWindow().SetWindowAttrib();
  }

  void EditorLayer::OnAttach()
  {
    m_EngineLogo = ResourceManager::Get().GetResourceAllocator()->GetResource<Texture2D>("assets/resource/DarkIcon.png");
    //m_Texture      = ResourceManager::Get().GetResourceAllocator()->GetResource<Texture2D>("assets/textures/Sprites/Tilesets/Fences.png");
    //m_TextureBlend = ResourceManager::Get().GetResourceAllocator()->GetResource<Texture2D>("assets/textures/Sprites/Tilesets/Grass.png");

    //m_Mesh = CreateRef<Mesh>();
    //m_Mesh->LoadFromFile("assets/models/cube.obj");

    m_Minimize = ResourceManager::Get().GetResourceAllocator()->GetResource<Texture2D>("assets/resource/subtract20.png");
    m_Restore  = ResourceManager::Get().GetResourceAllocator()->GetResource<Texture2D>("assets/resource/restore20.png");
    m_Close    = ResourceManager::Get().GetResourceAllocator()->GetResource<Texture2D>("assets/resource/close20.png");

    //m_MainCamera->SetOrthographic(3.0f, 0.1f, 100.0f);
    //m_MainCamera->SetViewMatrix(m_CameraController.GetCamera().GetViewMatrix());

    Dark::FramebufferSpecification fbSpec;
    fbSpec.Width            = 1280;
    fbSpec.Height           = 720;
    m_EditorViewFrameBuffer = Dark::Framebuffer::Create(fbSpec);
    m_MainCameraFrameBuffer = Dark::Framebuffer::Create(fbSpec);

    m_Scene = CreateRef<Scene>("SimpleScene1");

    //m_Shader    = ResourceManager::Get().s_ShaderLibrary->Get("Texture");
    //m_Texture   = ResourceManager::Get().GetResourceAllocator()->GetResource<Texture>("assets/textures/caodi.png");
    //m_renwuText = ResourceManager::Get().GetResourceAllocator()->GetResource<Texture>("assets/textures/renwu1.png");

    //auto entity2 = m_Scene->CreatEntity<Entity>("entity2");
    //entity2.AddComponent<MeshComponent>(ResourceManager::Get().s_PanelMesh);
    //entity2.AddComponent<MaterialComponent>(m_Shader, m_renwuText);

    //auto entity1 = m_Scene->CreatEntity<Entity>("entity1");
    //entity1.AddComponent<MeshComponent>(ResourceManager::Get().s_PanelMesh);
    //entity1.AddComponent<MaterialComponent>(m_Shader, m_Texture);

    //
    //
    //
    m_Serialize = CreateRef<Serialize>();
    m_Scene     = m_Serialize->DeserializeRuntime("assets/scenes/SimpleScene.ds");
    m_Serialize->SetSerializeScene(m_Scene);
    m_SceneHierarchy.SetContext(m_Scene);

    m_Audio = CreateRef<Audio>();
    m_Audio->InitDevice();
    m_Audio->SetSound("assets/Audio/test.mp3");
  }

  void EditorLayer::OnDetach()
  {
    DK_CORE_INFO("EditorLayer OnDetach");
  }

  void EditorLayer::OnUpdate(Dark::Timestep timestep)
  {
    //PROFILE_SCOPE("EditorLayer::OnUpdate");

    if (FramebufferSpecification spec = m_EditorViewFrameBuffer->GetSpecification();
        m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
        (spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
    {
      m_EditorViewFrameBuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);

      m_CameraController.OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);

      //m_MainCameraFrameBuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
      //m_MainCamera->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
    }
    //DK_TRACE("Delta Time: {0}s  {1}ms", timestep.GetSeconds(), timestep.GetMilliseconds());

    m_EditorViewFrameBuffer->Bind();

    if (m_ToolBar.GetIsRuntime())
    {
      if (!m_Audio->GetIsPlay())
        m_Audio->PlaySound();

      if (m_SceneMousePosX > 0.0f && m_SceneMousePosX < 1.0f && m_SceneMousePosY > 0.0f && m_SceneMousePosY < 1.0f)
      {
        m_Scene->OnUpdateRunTime(timestep, m_SceneMousePosX * m_ViewportSize.x, m_ViewportSize.y - m_SceneMousePosY * m_ViewportSize.y);
      }
      else
      {
        m_Scene->OnUpdateRunTime(timestep, 0.0f, 0.0f);
      }

      //m_Scene->OnUpdateRunTime(timestep);
    }
    else
    {
      if (m_Audio->GetIsPlay())
        m_Audio->StopSound();

      if (m_ViewPanelFocused)
        m_CameraController.OnUpdate(timestep);

      if (m_SceneMousePosX > 0.0f && m_SceneMousePosX < 1.0f && m_SceneMousePosY > 0.0f && m_SceneMousePosY < 1.0f)
      {
        m_Scene->OnUpdateEditor(m_CameraController.GetCamera(), timestep, m_SceneMousePosX * m_ViewportSize.x, m_ViewportSize.y - m_SceneMousePosY * m_ViewportSize.y);
      }
      else
      {
        m_Scene->OnUpdateEditor(m_CameraController.GetCamera(), timestep, 0.0f, 0.0f);
      }
    }

    m_EditorViewFrameBuffer->UnBind();

    if (m_SelectedEntity && m_SelectedEntity->HasComponent<CameraComponent>())
    {
      m_MainCameraFrameBuffer->Bind();
      m_Scene->OnUpdateRunTime(timestep, 0.0f, 0.0f);
      m_MainCameraFrameBuffer->UnBind();
    }

    m_SceneMousePosX = 0.0f;
    m_SceneMousePosY = 0.0f;
  }

  void EditorLayer::OnEvent(Event& event)
  {
    //DK_TRACE("{0}", event);
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<KeyPressedEvent>(DK_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
    dispatcher.Dispatch<MouseButtonPressedEvent>(DK_BIND_EVENT_FN(EditorLayer::OnMouseButtonPressed));
    dispatcher.Dispatch<MouseButtonReleasedEvent>(DK_BIND_EVENT_FN(EditorLayer::OnMouseButtonReleased));

    if (m_ViewPanelHovered && !m_ToolBar.GetIsRuntime())
      m_CameraController.OnEvent(event);
  }

  template <typename UIFunction>
  static void SceneToolbar(ImGuiDockNode* node, const float DISTANCE, int* corner, UIFunction uiFunc)
  {
    ImGuiIO& io                   = ImGui::GetIO();
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;

    ImVec2 work_area_pos  = node->Pos;
    ImVec2 work_area_size = node->Size;

    if (*corner != -1)
    {
      window_flags |= ImGuiWindowFlags_NoMove;
      ImGuiViewport* viewport = ImGui::GetMainViewport();

      ImVec2 window_pos       = ImVec2((*corner & 1) ? (work_area_pos.x + work_area_size.x - DISTANCE) : (work_area_pos.x + DISTANCE), (*corner & 2) ? (work_area_pos.y + work_area_size.y - DISTANCE) : (work_area_pos.y + DISTANCE));
      ImVec2 window_pos_pivot = ImVec2((*corner & 1) ? 1.0f : 0.0f, (*corner & 2) ? 1.0f : 0.0f);
      ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
      ImGui::SetNextWindowViewport(viewport->ID);
    }
    ImGui::SetNextWindowBgAlpha(0.3f); // Transparent background

    uiFunc(corner, work_area_size, window_flags);
  }

  void EditorLayer::OnImGuiRender()
  {
    static bool opt_fullscreen                = true;
    static bool opt_padding                   = false;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None | ImGuiDockNodeFlags_NoWindowMenuButton;
    static bool* p_open;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 7));
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen)
    {
      ImGuiViewport* viewport = ImGui::GetMainViewport();
      ImGui::SetNextWindowPos(viewport->WorkPos);
      ImGui::SetNextWindowSize(viewport->WorkSize);
      ImGui::SetNextWindowViewport(viewport->ID);
      ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
      ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
      window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
      window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }
    else
    {
      dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
    }

    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
    // and handle the pass-thru hole, so we ask Begin() to not render a background.
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
      window_flags |= ImGuiWindowFlags_NoBackground;

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    if (!opt_padding)
      ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", p_open, window_flags);
    if (!opt_padding)
      ImGui::PopStyleVar();

    if (opt_fullscreen)
      ImGui::PopStyleVar(2);

    ImVec2 availSize = ImGui::GetContentRegionAvail();

    if (ImGui::BeginMenuBar())
    {
      ImGui::Image((ImTextureID)m_EngineLogo->GetRendererID(), ImVec2(m_EngineLogo->GetHeight(), m_EngineLogo->GetWidth()));

      if (ImGui::BeginMenu("File"))
      {
        // Disabling fullscreen would allow the window to be moved to the front of other windows,
        // which we can't undo at the moment without finer window depth/z control.
        if (ImGui::MenuItem("Save"))
          m_Serialize->SerializeRuntime("assets/scenes/SimpleScene.ds");
        if (ImGui::MenuItem("Exit"))
          Dark::Application::Get().Exit();

        ImGui::EndMenu();
      }

      if (ImGui::BeginMenu("Edit"))
      {
        ImGui::EndMenu();
      }

      if (ImGui::BeginMenu("Windows"))
      {
        ImGui::EndMenu();
      }

      if (ImGui::BeginMenu("Help"))
      {
        ImGui::EndMenu();
      }

      //ImGui::SameLine(ImGui::GetContentRegionAvailWidth() / 2, 0.0f);

      //ImGui::SetCursorPosY(curCursorPos.y + off);
      ImDrawList* drawList = ImGui::GetWindowDrawList();

      //ImVec2 p0 = ImGui::GetCursorScreenPos();

      drawList->AddRectFilled(ImVec2(availSize.x / 1.3, 2), ImVec2(availSize.x / 1.3 + ImGui::CalcTextSize("SandBox").x + 20, 30), ImGui::GetColorU32(IM_COL32(10, 5, 10, 255)));
      drawList->AddText(ImGui::GetFont(), 18, ImVec2(availSize.x / 1.3 + 5, 6), ImGui::GetColorU32(IM_COL32(255, 255, 255, 255)), "SandBox");

      ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.2f, 0.205f, 0.21f, 1.0f});
      ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.3f, 0.305f, 0.31f, 1.0f});
      ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.15f, 0.1505f, 0.151f, 1.0f});
      ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(12, 7));
      //ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 8.0f);
      ImGui::SameLine(availSize.x - (44 * 3));
      if (ImGui::ImageButton((void*)m_Minimize->GetRendererID(), ImVec2(m_Minimize->GetWidth(), m_Minimize->GetHeight()), ImVec2(0, 1), ImVec2(1, 0), -1.0f, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(1.0f, 1.0f, 1.0f, 0.7f)))
      {
        Dark::Application::Get().GetWindow().SetMinimize();
      }
      ImGui::SameLine(0, 0);
      if (ImGui::ImageButton((void*)m_Restore->GetRendererID(), ImVec2(m_Restore->GetWidth(), m_Restore->GetHeight()), ImVec2(0, 1), ImVec2(1, 0), -1.0f, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(1.0f, 1.0f, 1.0f, 0.7f)))
      {
        Dark::Application::Get().GetWindow().SetMaximizeOrRestore();
      }
      ImGui::SameLine(0, 0);
      if (ImGui::ImageButton((void*)m_Close->GetRendererID(), ImVec2(m_Close->GetWidth(), m_Close->GetHeight()), ImVec2(0, 1), ImVec2(1, 0), -1.0f, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(1.0f, 1.0f, 1.0f, 0.7f)))
      {
        Dark::Application::Get().Exit();
      }
      ImGui::PopStyleVar();
      ImGui::PopStyleColor(3);

      ImGui::EndMenuBar();
    }
    ImGui::PopStyleVar(1);

    // DockSpace
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
      ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
      ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    //m_ToolBar.OnImGuiRender();

    // Scene
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0.0, 0.0});
    ImGui::Begin("Scene");

    m_SceneWindowPosX   = ImGui::GetWindowPos().x;
    m_SceneWindowPosY   = ImGui::GetWindowPos().y;
    m_SceneWindowWidth  = ImGui::GetWindowWidth();
    m_SceneWindowHeight = ImGui::GetWindowHeight();

    ImGuiDockNode* node = ImGui::GetWindowDockNode();
    node->LocalFlags |= ImGuiDockNodeFlags_NoTabBar;

    m_ViewPanelFocused       = ImGui::IsWindowFocused();
    m_ViewPanelHovered       = ImGui::IsWindowHovered();
    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
    m_ViewportSize           = {viewportPanelSize.x, viewportPanelSize.y};

    ImGui::Image((void*)m_EditorViewFrameBuffer->GetColorAttachmentRendererID(), ImVec2{m_ViewportSize.x, m_ViewportSize.y}, ImVec2{0.0f, 1.0f}, ImVec2{1.0f, 0.0f});

    // ImGuizmo
    if (m_SelectedEntity)
    {
      ImGuizmo::SetOrthographic(false);
      ImGuizmo::SetDrawlist();

      float windowWidth  = (float)ImGui::GetWindowWidth();
      float windowHeight = (float)ImGui::GetWindowHeight();

      ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

      glm::mat4 cameraProjection;
      glm::mat4 cameraView;
      //glm::mat4 cameraView = glm::inverse(m_CameraController.GetCamera().GetTransform());
      if (m_ToolBar.GetIsRuntime())
      {
        //cameraProjection = m_MainCamera->GetProjectionMatrix();
        //cameraView       = m_MainCamera->GetViewMatrix();
      }
      else
      {
        cameraProjection = m_CameraController.GetCamera().GetProjectionMatrix();
        cameraView       = m_CameraController.GetCamera().GetViewMatrix();
      }

      auto& tc = m_SelectedEntity->GetComponent<TransformComponent>();

      glm::mat4 transformation = tc.GetTransform();

      ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection), (ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transformation));

      if (ImGuizmo::IsUsing())
      {
        glm::vec3 translation, rotation, scale;
        Math::DecomposeTransform(transformation, translation, rotation, scale);

        tc.Translation          = translation;
        glm::vec3 deltaRotation = rotation - tc.Rotation;
        tc.Rotation += deltaRotation;
        tc.Scale = scale;
      }
    }

    static int settingCorner = 3;

    SceneToolbar(node, 10.0f, &settingCorner, [&](int* corner, const ImVec2& work_area_size, const ImGuiWindowFlags m_window_flags) {
      ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

      float buttonSize = 25.0f;
      if (ImGui::Begin("Setting Toolbar", p_open, m_window_flags))
      {
        if (work_area_size.x < 100)
        {
          buttonSize = work_area_size.x / 4;
        }

        if (m_SelectedEntity && m_SelectedEntity->HasComponent<CameraComponent>())
        {
          // MainCamera preview
          ImGui::Text("MainCamera");
          ImGui::Separator();
          ImGui::Image((void*)m_MainCameraFrameBuffer->GetColorAttachmentRendererID(), ImVec2{m_ViewportSize.x / 4, m_ViewportSize.y / 4}, ImVec2{0.0f, 1.0f}, ImVec2{1.0f, 0.0f});
        }

        /*
		  ImGui::Text("Simple overlay\n" "in the corner of the screen.\n" "(right-click to change position)");
		  ImGui::Separator();
		  if (ImGui::IsMousePosValid())
			ImGui::Text("Mouse Position: (%.1f,%.1f)", io.MousePos.x, io.MousePos.y);
		  else
			ImGui::Text("Mouse Position: <invalid>");
		  if (ImGui::BeginPopupContextWindow())
		  {
			if (ImGui::MenuItem("Custom", NULL, *corner == -1)) *corner = -1;
			if (ImGui::MenuItem("Top-left", NULL, *corner == 0)) *corner = 0;
			if (ImGui::MenuItem("Top-right", NULL, *corner == 1)) *corner = 1;
			if (ImGui::MenuItem("Bottom-left", NULL, *corner == 2)) *corner = 2;
			if (ImGui::MenuItem("Bottom-right", NULL, *corner == 3)) *corner = 3;
			if (p_open && ImGui::MenuItem("Close")) *p_open = false;
			ImGui::EndPopup();
		  }
		  */
      }

      ImGui::End();
      ImGui::PopStyleVar();
    });

    ImGui::End();
    ImGui::PopStyleVar();

    static bool detail_open = false;
    // Detail
    ImGui::Begin("Detail", &detail_open);
    ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
    ImGui::End();

    //Setting
    ImGui::Begin("Setting");
    //Camera Rotation
    //ImGui::DragFloat3("Camera Rotation", glm::value_ptr(m_CameraRotation), 0.03f);
    ImGui::End();

    m_ECSPanel.OnImGuiRender();
    m_Content.OnImGuiRender();
    m_SceneHierarchy.OnImGuiRender();
    m_ToolBar.OnImGuiRender();

    ImGui::End();

    /*
    {
      ImGuiWindowClass window_class2;
      window_class2.ClassId                  = ImGui::GetID("XXX");
      window_class2.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoDockingSplitOther | ImGuiDockNodeFlags_NoDockingSplitMe;
      window_class2.DockingAllowUnclassed    = false;

      //-----------------Window2------------------------------
      ImGui::SetNextWindowClass(&window_class2);
      ImGui::Begin("##Window2", nullptr, ImGuiWindowFlags_NoCollapse);

      ImGuiID dockSpace2 = ImGui::GetID("dockSpace2");
      ImGui::DockSpace(dockSpace2);

      const ImGuiWindow* window = ImGui::GetCurrentWindow();
      const ImRect titleBarRect = window->TitleBarRect();
      ImGui::PushClipRect(titleBarRect.Min, titleBarRect.Max, false);
      ImGui::SetCursorPos(ImVec2(80.0f, 0.0f));
      // button works fine until the window is docked
      ImGui::Button("Overlapping Button");
      ImGui::PopClipRect();

      ImGui::End();

      ImGuiWindowClass window_class2_for_subwindow;
      window_class2_for_subwindow.ClassId                  = ImGui::GetID("Window2");
      window_class2_for_subwindow.DockNodeFlagsOverrideSet = 0;
      window_class2_for_subwindow.DockingAllowUnclassed    = true;

      //-----------------Window2_A------------------------------
      ImGui::SetNextWindowClass(&window_class2_for_subwindow);
      ImGui::Begin("Window2_A");
      ImGui::End();

      //-----------------Window2_B------------------------------
      ImGui::SetNextWindowClass(&window_class2_for_subwindow);
      ImGui::Begin("Window2_B");
      ImGui::End();

      //-----------------Window2_C------------------------------
      ImGui::SetNextWindowClass(&window_class2_for_subwindow);
      ImGui::Begin("Window2_C");
      ImGui::End();
    }

    {
      //-----------------window3------------------------------
      ImGuiWindowClass window_class3;
      window_class3.ClassId                  = ImGui::GetID("XXX");
      window_class3.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoDockingSplitOther | ImGuiDockNodeFlags_NoDockingSplitMe;
      window_class3.DockingAllowUnclassed    = false;
      ImGui::SetNextWindowClass(&window_class3);
      ImGui::Begin("Window3");

      ImGuiID dockSpace3 = ImGui::GetID("dockSpace3");
      ImGui::DockSpace(dockSpace3);

      ImGui::End();

      ImGuiWindowClass window_class3_for_subwindow;
      window_class3_for_subwindow.ClassId                  = ImGui::GetID("Window3");
      window_class3_for_subwindow.DockNodeFlagsOverrideSet = 0;
      window_class3_for_subwindow.DockingAllowUnclassed    = true;

      //-----------------Window3_A------------------------------
      ImGui::SetNextWindowClass(&window_class3_for_subwindow);
      ImGui::Begin("Window3_A");
      ImGui::End();

      //-----------------Window3_B------------------------------
      ImGui::SetNextWindowClass(&window_class3_for_subwindow);
      ImGui::Begin("Window3_B");
      ImGui::End();
    }
    */
  }

  bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
  {
    if (e.GetRepeatCount() > 0)
      return false;

    switch (e.GetKeyCode())
    {
    case DK_KEY_SPACE: {
      if (m_GizmoType < 2)
        ++m_GizmoType;
      else
        m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;

      break;
    }
    }
  }

  bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
  {
    if (e.GetMouseButton() == DK_MOUSE_BUTTON_LEFT)
    {
      m_SceneMousePosX = (ImGui::GetMousePos().x - m_SceneWindowPosX) / m_SceneWindowWidth;
      m_SceneMousePosY = (ImGui::GetMousePos().y - m_SceneWindowPosY) / m_SceneWindowHeight;

      DK_CORE_INFO("POS:{0}, {1}", m_SceneMousePosX, m_SceneMousePosY);
    }

    return false;
  }

  bool EditorLayer::OnMouseButtonReleased(MouseButtonReleasedEvent& e)
  {
    m_SelectedEntity = m_Scene->GetSelectEntity();

    if (m_SelectedEntity)
    {
      m_SceneHierarchy.SetSelectEntity(*m_SelectedEntity);
    }

    return false;
  }

} // namespace Dark