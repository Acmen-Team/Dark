#include "dkpch.h"

#include "Texture.h"

#include "Dark/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLTexture.h"

namespace Dark {

  //Ref<Texture2D> Texture2D::Create(const std::string& path)
  //{
  //  switch (Renderer::GetAPI())
  //  {
  //  case RendererAPI::API::None:
  //    DK_CORE_ASSERT(false, "RendererAPI::None is not supported!");
  //    return nullptr;
  //  case RendererAPI::API::OpenGL:
  //    return CreateRef<OpenGLTexture2D>(path);
  //  }

  //  DK_CORE_ASSERT(false, "Unkonw RendererAPI!");
  //  return nullptr;
  //}

  std::pair<Ref<ResourceID>, Ref<Resource>> Texture2D::LoadFromFile(const std::string& path)
  {
    switch (Renderer::GetAPI())
    {
    case RendererAPI::API::None: {
      DK_CORE_ASSERT(false, "RendererAPI::None is not supported!");
    }
    case RendererAPI::API::OpenGL: {
      this->m_Res   = CreateRef<OpenGLTexture2D>(path);
      this->m_ResID = CreateRef<ResourceID>(path);
    }
    }
    return std::pair<Ref<ResourceID>, Ref<Resource>>(this->m_ResID, this->m_Res);
  }

  //{
  //  switch (Renderer::GetAPI())
  //  {
  //  case RendererAPI::API::None: {
  //    DK_CORE_ASSERT(false, "RendererAPI::None is not supported!");
  //    this->m_Res = nullptr;
  //    this->m_ResID = nullptr;

  //    return std::pair<ResourceID*, Resource*>{this->m_ResID, this->m_Res};
  //  }
  //  case RendererAPI::API::OpenGL: {
  //    Resource::m_Res   = new OpenGLTexture2D(path);
  //    Resource::m_ResID = new ResourceID(path);

  //    return std::pair<ResourceID*, Resource*>{Resource::m_ResID, Resource::m_Res};
  //  }
  //  //case RendererAPI::API::Vulkan: {
  //  //}
  //  }

  //  DK_CORE_ASSERT(false, "Unkonw RendererAPI!");
  //  return std::pair<Ref<ResourceID>, Ref<Resource>>(nullptr, nullptr);
  //}

  //Ref<Texture2D> Texture2D::LoadFromFile(const std::string& path)
  //{
  //  switch (Renderer::GetAPI())
  //  {
  //  case RendererAPI::API::None:
  //    DK_CORE_ASSERT(false, "RendererAPI::None is not supported!");
  //    return nullptr;
  //  case RendererAPI::API::OpenGL:
  //    return CreateRef<OpenGLTexture2D>(path);
  //  }
  //
  //  DK_CORE_ASSERT(false, "Unkonw RendererAPI!");
  //  return nullptr;
  //}

} // namespace Dark