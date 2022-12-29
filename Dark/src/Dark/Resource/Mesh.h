/*************************************************
Copyright © 2020 - 2022 YX.All Rights Reserved

Date:2022-11-2 20:56
Description:Mesh Resource
**************************************************/
#pragma once

#include "Dark/Renderer/VertexArray.h"

#include "Dark/Core/Resource.h"

#include "glm/glm.hpp"

namespace Dark {

  struct Vertex
  {
    glm::vec3 _Position;
    glm::vec3 _Normal;
    glm::vec2 _TexCoords;

    Vertex(glm::vec3& pos, glm::vec3& normal, glm::vec2& texcoord)
    {
      _Position  = pos;
      _Normal    = normal;
      _TexCoords = texcoord;
    }
  };

  struct ShapeData
  {
    std::string _Name;
    std::vector<unsigned int> _IndicesList;
    std::vector<Vertex> _VerticesList;
  };

  class Mesh : public Resource
  {
  public:
    Mesh()
    {
      //DK_CORE_INFO("Mesh()");
    }
    virtual ~Mesh() = default;

    std::string& GetMeshName() { return m_Name; }
    std::map<std::string, Ref<VertexArray>>& GetMesh();

    virtual std::pair<Ref<ResourceID>, Ref<Resource>> LoadFromFile(const std::string& path) override;
    virtual std::pair<Ref<ResourceID>, Ref<Resource>> LoadFromMemory(void* mem, size_t size) override
    {
      return std::pair<Ref<ResourceID>, Ref<Resource>>(this->m_ResID, this->m_Res);
    }

  private:
    void ObjLoad(const std::string& filePath, const std::string& base_path, bool triangle = true);
    void CreatVertexArrayList();

  protected:
    std::string m_Name;
    std::map<std::string, std::shared_ptr<VertexArray>> m_VertexArrayList;

  private:
    std::vector<Ref<ShapeData>> _Shapes;
  };

  class PanelMesh : public Mesh
  {
  public:
    PanelMesh()
    {
      m_Name = std::string("Panel");

      Ref<VertexArray> vertexArray;
      Ref<VertexBuffer> vertexBuffer;
      Ref<IndexBuffer> indexBuffer;

      vertexArray.reset(VertexArray::Create());

      vertexBuffer.reset(VertexBuffer::Create(m_Vertices, sizeof(m_Vertices)));
      BufferLayout layout = {
          {ShaderDataType::Float3, "a_Pos"},
          {ShaderDataType::Float2, "a_TexCoord"}};
      vertexBuffer->SetLayout(layout);
      vertexArray->AddVertexBuffer(vertexBuffer);

      indexBuffer.reset(IndexBuffer::Create(m_Indices, sizeof(m_Indices) / sizeof(uint32_t)));
      vertexArray->SetIndexBuffer(indexBuffer);

      m_VertexArrayList.insert(std::pair<std::string, Ref<VertexArray>>(m_Name, vertexArray));
    }

  private:
    float m_Vertices[20] = {
        0.5f, 0.5f, 0.0f, 1.0f, 1.0f,   // 右上角
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  // 右下角
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // 左下角
        -0.5f, 0.5f, 0.0f, 0.0f, 1.0f   // 左上角
    };

    uint32_t m_Indices[6] = {
        0, 1, 3, // 第一个三角形
        1, 2, 3  // 第二个三角形
    };
  };

} // namespace Dark