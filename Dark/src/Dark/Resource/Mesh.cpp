#include "dkpch.h"

#include "Mesh.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

namespace Dark {

  std::map<std::string, Ref<VertexArray>>& Mesh::GetMesh()
  {
    if (m_VertexArrayList.size() == 0)
      CreatVertexArrayList();

    return m_VertexArrayList;
  }

  std::pair<Ref<ResourceID>, Ref<Resource>> Mesh::LoadFromFile(const std::string& path)
  {
    // can be create?
    auto lastSlash = path.find_last_of("/\\");
    lastSlash      = lastSlash == std::string::npos ? 0 : lastSlash + 1;
    auto basePath  = path.substr(0, lastSlash + 1);
    auto lastDot   = path.rfind('.');
    auto count     = lastDot == std::string::npos ? path.size() - lastSlash : lastDot - lastSlash;

    this->m_Name = path.substr(lastSlash, count);
    this->ObjLoad(path, basePath);

    return std::pair<Ref<ResourceID>, Ref<Resource>>(this->m_ResID, this->m_Res);
  }

  void Mesh::ObjLoad(const std::string& filePath, const std::string& base_path, bool triangle /*= true*/)
  {
    tinyobj::ObjReaderConfig reader_config;
    reader_config.mtl_search_path = base_path; // Path to material files

    tinyobj::ObjReader reader;

    if (!reader.ParseFromFile(filePath, reader_config))
    {
      if (!reader.Error().empty())
      {
        DK_CORE_ERROR("TinyObjReader:{0}", reader.Error());
      }
      exit(1);
    }

    if (!reader.Warning().empty())
    {
      DK_CORE_WARN("TinyObjReader:{0}", reader.Warning());
    }

    glm::vec3 pos      = {0.0f, 0.0f, 0.0f};
    glm::vec3 normal   = {0.0f, 0.0f, 0.0f};
    glm::vec2 texCoord = {0.0f, 0.0f};

    auto& attrib    = reader.GetAttrib();
    auto& shapes    = reader.GetShapes();
    auto& materials = reader.GetMaterials();

    // Loop over shapes
    for (size_t s = 0; s < shapes.size(); s++)
    {
      // Loop over faces(polygon)
      Ref<ShapeData> shape = CreateRef<ShapeData>();

      shape->_Name = shapes[s].name;

      size_t index_offset = 0;
      for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
      {

        //pro = (float)f / (float)shapes[s].mesh.num_face_vertices.size();

        //DK_CORE_TRACE("PRO : {0}", pro);

        size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);

        // Loop over vertices in the face.
        for (size_t v = 0; v < fv; v++)
        {
          // access to vertex
          tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
          pos.x                = attrib.vertices[3 * size_t(idx.vertex_index) + 0];
          pos.y                = attrib.vertices[3 * size_t(idx.vertex_index) + 1];
          pos.z                = attrib.vertices[3 * size_t(idx.vertex_index) + 2];

          // Check if `normal_index` is zero or positive. negative = no normal data
          if (idx.normal_index >= 0)
          {
            normal.x = attrib.normals[3 * size_t(idx.normal_index) + 0];
            normal.y = attrib.normals[3 * size_t(idx.normal_index) + 1];
            normal.z = attrib.normals[3 * size_t(idx.normal_index) + 2];
          }

          // Check if `texcoord_index` is zero or positive. negative = no texcoord data
          if (idx.texcoord_index >= 0)
          {
            texCoord.x = attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
            texCoord.y = attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];
          }

          // Optional: vertex colors
          // tinyobj::real_t red   = attrib.colors[3*size_t(idx.vertex_index)+0];
          // tinyobj::real_t green = attrib.colors[3*size_t(idx.vertex_index)+1];
          // tinyobj::real_t blue  = attrib.colors[3*size_t(idx.vertex_index)+2];

          Vertex vertices(pos, normal, texCoord);
          shape->_IndicesList.push_back(index_offset + v);
          shape->_VerticesList.push_back(vertices);
        }
        index_offset += fv;

        // per-face material
        shapes[s].mesh.material_ids[f];
      }
      _Shapes.push_back(shape);
    }
  }

  void Mesh::CreatVertexArrayList()
  {
    Ref<VertexArray> vertexArray;
    Ref<VertexBuffer> vertexBuffer;
    Ref<IndexBuffer> indexBuffer;

    for (auto shape : _Shapes)
    {
      vertexArray.reset(VertexArray::Create());

      vertexBuffer.reset(VertexBuffer::Create((float*)&shape->_VerticesList[0], sizeof(float) * 8 * shape->_VerticesList.size()));

      BufferLayout MeshLayout = {
          {ShaderDataType::Float3, "a_Pos"},
          {ShaderDataType::Float3, "a_Normal"},
          {ShaderDataType::Float2, "a_TexCoord"}};

      vertexBuffer->SetLayout(MeshLayout);
      vertexArray->AddVertexBuffer(vertexBuffer);

      indexBuffer.reset(IndexBuffer::Create((uint32_t*)&shape->_IndicesList[0], shape->_IndicesList.size()));
      vertexArray->SetIndexBuffer(indexBuffer);

      m_VertexArrayList.insert(std::pair<std::string, Ref<VertexArray>>(shape->_Name, vertexArray));
    }
  }

} // namespace Dark