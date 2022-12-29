#include "dkpch.h"

#include "SceneHierarchyPanel.h"

#include <glm/gtc/type_ptr.hpp>

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

namespace Dark {

  SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context)
  {
    SetContext(context);
  }

  void SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
  {
    m_Context = context;
  }

  void SceneHierarchyPanel::OnImGuiRender()
  {
    ImGui::Begin("Scene Hierarchy");

    ImGuiDockNode* nodeScene = ImGui::GetWindowDockNode();
    if (nodeScene)
      nodeScene->LocalFlags |= ImGuiDockNodeFlags_NoCloseButton;

    m_Context->m_Registry.each([&](auto entityID) {
      Entity entity{entityID, m_Context.get()};

      DrawEntityNode(entity);
    });

    if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
    {
      m_SelectionContext = {};
    }

    // Add Entity
    if (ImGui::BeginPopupContextWindow(0, 1, false))
    {
      if (ImGui::MenuItem("Create Empty Entity"))
        m_Context->CreatEntity<Entity>("Empty Entity");
      if (ImGui::MenuItem("Create Light"))
      {
        //LightEntity light = m_Context->CreatEntity<LightEntity>("Light");

        //light.AddComponent<LightPropertyComponent>(light.GetProperty());
        //light.AddComponent<MeshComponent>(ResourceManager::Get().GetMeshAllocator()->GetRes("assets/models/bulb.obj"));
        //light.GetComponent<TransformComponent>().Scale -= glm::vec3(0.9, 0.9, 0.9);
      }
      ImGui::EndPopup();
    }

    ImGui::End();

    ImGui::Begin("Properties");

    ImGuiDockNode* nodeProperties = ImGui::GetWindowDockNode();
    if (nodeProperties)
      nodeProperties->LocalFlags |= ImGuiDockNodeFlags_NoCloseButton;

    if (m_SelectionContext)
    {
      DrawComponents(m_SelectionContext);
    }

    ImGui::End();
  }

  void SceneHierarchyPanel::DrawEntityNode(Entity entity)
  {
    auto& tag = entity.GetComponent<TagComponent>().Tag;

    ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
    flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
    bool opened = ImGui::TreeNodeEx((void*)(uint32_t)entity, flags, tag.c_str());
    if (ImGui::IsItemClicked())
    {
      m_SelectionContext = entity;
    }

    bool entityDeleted = false;
    if (ImGui::BeginPopupContextItem())
    {
      if (ImGui::MenuItem("Delete Entity"))
        entityDeleted = true;

      ImGui::EndPopup();
    }

    if (opened)
    {
      //ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
      //bool opened              = ImGui::TreeNodeEx((void*)758963, flags, tag.c_str());
      //if (opened)
      //  ImGui::TreePop();
      ImGui::TreePop();
    }

    if (entityDeleted)
    {
      m_Context->DestroyEntity(entity);
      if (m_SelectionContext == entity)
        m_SelectionContext = {};
    }
  }

  static void DrawVec3Control(const std::string& label, glm::vec3& value, float resetValue = 0.0f, float columnWidth = 70.0f)
  {
    ImGuiIO& io   = ImGui::GetIO();
    auto boldFont = io.Fonts->Fonts[0];

    ImGui::PushID(label.c_str());

    ImGui::Columns(2);
    ImGui::SetColumnWidth(0, columnWidth);
    ImGui::Text(label.c_str());
    ImGui::NextColumn();

    ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{0, 0});

    float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;

    ImVec2 buttonSize = {lineHeight + 3.0f, lineHeight};

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.9f, 0.2f, 0.2f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
    ImGui::PushFont(boldFont);
    if (ImGui::Button("X", buttonSize))
      value.x = resetValue;
    ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::DragFloat("##X", &value.x, 0.1f, 0.0f, 0.0f, "%.3f");
    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.3f, 0.8f, 0.3f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
    ImGui::PushFont(boldFont);
    if (ImGui::Button("Y", buttonSize))
      value.y = resetValue;
    ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::DragFloat("##Y", &value.y, 0.1f, 0.0f, 0.0f, "%.3f");
    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.1f, 0.25f, 0.8f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.2f, 0.35f, 0.9f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.1f, 0.25f, 0.8f, 1.0f});
    ImGui::PushFont(boldFont);
    if (ImGui::Button("Z", buttonSize))
      value.z = resetValue;
    ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::DragFloat("##Z", &value.z, 0.1f, 0.0f, 0.0f, "%.3f");
    ImGui::PopItemWidth();

    ImGui::PopStyleVar();
    ImGui::Columns(1);

    ImGui::PopID();
  }

  const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

  template <typename T, typename UIFunction>
  static void DrawComponent(const std::string& name, Entity entity, bool hasSetting, UIFunction uiFunc)
  {
    if (entity.HasComponent<T>())
    {
      auto& component               = entity.GetComponent<T>();
      ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

      ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{4, 4});
      float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
      ImGui::Separator();
      bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());

      if (hasSetting)
      {
        ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);

        if (ImGui::Button("+", ImVec2{lineHeight, lineHeight}))
        {
          ImGui::OpenPopup("ComponentSetting");
        }

        bool removeComponent = false;
        if (ImGui::BeginPopup("ComponentSetting"))
        {
          if (ImGui::MenuItem("Remove component"))
            removeComponent = true;

          ImGui::EndPopup();
        }

        if (removeComponent)
          entity.RemoveComponent<T>();
      }

      ImGui::PopStyleVar();

      if (open)
      {
        uiFunc(component);
        ImGui::TreePop();
      }
    }
  }

  void SceneHierarchyPanel::DrawComponents(Entity entity)
  {
    if (entity.HasComponent<TagComponent>())
    {
      auto& tag = entity.GetComponent<TagComponent>().Tag;

      char buffer[256];
      memset(buffer, 0, sizeof(buffer));
      strcpy_s(buffer, sizeof(buffer), tag.c_str());
      ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);
      if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
      {
        tag = std::string(buffer);
      }
      ImGui::PopStyleVar();
    }

    ImGui::SameLine();
    ImGui::PushItemWidth(-1);

    if (ImGui::Button("Add Component"))
      ImGui::OpenPopup("AddComponent");

    if (ImGui::BeginPopup("AddComponent"))
    {
      if (ImGui::MenuItem("Mesh"))
      {
        m_SelectionContext.AddComponent<MeshComponent>(ResourceManager::Get().s_PanelMesh);
        ImGui::CloseCurrentPopup();
      }

      if (ImGui::MenuItem("Material"))
      {
        m_SelectionContext.AddComponent<MaterialComponent>(ResourceManager::Get().s_ShaderLibrary->Get("Texture"), ResourceManager::Get().GetResourceAllocator()->GetResource<Texture>("assets/textures/caodi.png"));
        ImGui::CloseCurrentPopup();
      }

      ImGui::EndPopup();
    }

    ImGui::PopItemWidth();

    if (entity.HasComponent<TransformComponent>())
    {
      if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), treeNodeFlags, "Transform"))
      {
        auto& Translation = entity.GetComponent<TransformComponent>().Translation;
        auto& Rotation    = entity.GetComponent<TransformComponent>().Rotation;
        auto& Scale       = entity.GetComponent<TransformComponent>().Scale;

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(ImGui::GetStyle().ItemSpacing.x, 8));
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);
        DrawVec3Control("Position", Translation);
        glm::vec3 rotation = glm::degrees(Rotation);
        DrawVec3Control("Rotation", rotation);
        Rotation = glm::radians(rotation);
        DrawVec3Control("Scale", Scale, 1.0f);
        ImGui::PopStyleVar(2);

        ImGui::TreePop();
      }
    }

    //if (entity.HasComponent<LightPropertyComponent>())
    //{
    //  DrawCommponent<LightPropertyComponent>("LightProperty", entity, false, [](auto& component) {
    //    ImGui::AlignTextToFramePadding();
    //    ImGui::Text("LightType:");
    //    ImGui::SameLine();

    //    ImGui::Combo("##LightType", &component._Property->LightType, "Direction Light\0Point Light\0Sport Light\0\0");

    //    // TODO: Add custom color button

    //    ImGui::ColorEdit4("Color", glm::value_ptr(component._Property->Color));
    //    ImGui::DragFloat("Intensity", &component._Property->Intensity);

    //    if (0 == component._Property->LightType)
    //    {
    //    }
    //    else if (1 == component._Property->LightType)
    //    {
    //      ImGui::DragFloat("Range", &component._Property->Range);
    //    }
    //    else if (2 == component._Property->LightType)
    //    {
    //      ImGui::DragFloat("Range", &component._Property->Range);
    //      ImGui::DragFloat("SpotAngle", &component._Property->SpotAngle);
    //    }
    //  });

    //  return;
    //}

    DrawComponent<CameraComponent>("Camera", entity, true, [](auto& component) {
      const char* projectionTypeStrings[]     = {"Perspective",
                                             "Orthographic"};
      const char* currentProjectionTypeString = projectionTypeStrings[(int)component._Camera->GetProjectionType()];

      ImGui::Checkbox("Fixed Aspect Ratio", &component._Camera->fixedAspectration);

      if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
      {
        for (int i = 0; i < 2; i++)
        {
          bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
          if (ImGui::Selectable(projectionTypeStrings[i], isSelected))
          {
            currentProjectionTypeString = projectionTypeStrings[i];
            component._Camera->SetProjectionType((ProjectionType)i);
          }

          if (isSelected)
            ImGui::SetItemDefaultFocus();
        }

        ImGui::EndCombo();
      }
      // Setting perspective camera
      if (component._Camera->GetProjectionType() == ProjectionType::ePerspective)
      {
        float persFov = component._Camera->GetPerspectiveFov();
        if (ImGui::DragFloat("Fov", &persFov, 0.05f, 0.1f))
          component._Camera->SetPerspectiveFov(persFov);

        float persNear = component._Camera->GetPerspectiveNear();
        if (ImGui::DragFloat("Near", &persNear))
          component._Camera->SetPerspectiveNear(persNear);

        float persFar = component._Camera->GetPerspectiveFar();
        if (ImGui::DragFloat("Far", &persFar))
          component._Camera->SetPerspectiveFar(persFar);
      }
      // Setting ortho camera
      if (component._Camera->GetProjectionType() == ProjectionType::eOrthographic)
      {
        float orthoSize = component._Camera->GetOrthographicSize();
        if (ImGui::DragFloat("Size", &orthoSize, 0.05f, 0.1f, 50.0f))
          component._Camera->SetOrthographicSize(orthoSize);

        float orthoNear = component._Camera->GetOrthographicNear();
        if (ImGui::DragFloat("Near", &orthoNear))
          component._Camera->SetOrthographicNear(orthoNear);

        float orthoFar = component._Camera->GetOrthographicFar();
        if (ImGui::DragFloat("Far", &orthoFar))
          component._Camera->SetOrthographicFar(orthoFar);
      }
    });

    DrawComponent<MeshComponent>("Mesh", entity, true, [](auto& component) {
      std::string meshName = "";
      int size             = ResourceManager::Get().GetResourceAllocator()->GetResSize();

      meshName += component._Mesh->GetMeshName() + '\0';
      //for (int i = 0; i < size; i++)
      //{
      //  meshName += ResourceManager::Get().GetMeshAllocator()->GetRes(i)->GetMeshName() + '\0';
      //}
      //component._Mesh = ResourceManager::Get().GetMeshAllocator()->GetRes(m_CurrentMesh);

      //ImGui::Combo("##MeshResource", 0, component._Mesh->GetMeshName());

      ImGui::Text("Mesh Name:%s", component._Mesh->GetMeshName().c_str());
      //ImGui::LabelText("Name:", "%s", component._Mesh->GetMeshName().c_str());

      //component._Id = m_CurrentMesh;
    });

    DrawComponent<MaterialComponent>("Material", entity, true, [](auto& component) {
      ImGui::Text("Shader Name:%s", component._Shader->GetName().c_str());
      if (ImGui::ImageButton((void*)component._Texture->GetRendererID(), ImVec2(component._Texture->GetWidth(), component._Texture->GetHeight()), ImVec2(0, 1), ImVec2(1, 0)))
      {
        // TODO: Show All Textures

        //auto& resBeginIter = ResourceManager::Get().GetResourceAllocator()->GetResourcesBegin();
        //for (int i = 0; i < ResourceManager::Get().GetResourceAllocator()->GetResSize(); i++)
        //{
        //  if (resBeginIter->first->type == ResourceType::eTexture)
        //  {
        //    DK_CORE_INFO("Texture:{0}", resBeginIter->first->name);
        //  }

        //  resBeginIter++;
        //}
      }

      if (ImGui::BeginDragDropTarget())
      {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM_TEXTURE"))
        {
          DK_CORE_ASSERT(!payload->DataSize > 0, "ERROR Drop Source");

          std::string path = std::string((const char*)payload->Data, payload->DataSize);

          if (ResourceManager::Get().GetResourceAllocator()->HasRes(path))
          {
            component._Texture = ResourceManager::Get().GetResourceAllocator()->GetResource<Texture>(path);
          }

          DK_CORE_INFO("Drop Souce Path:{0}", path.c_str());
        }
        ImGui::EndDragDropTarget();
      }
    });
  }

} // namespace Dark