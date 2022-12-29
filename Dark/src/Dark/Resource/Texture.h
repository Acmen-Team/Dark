#pragma once

#include "Dark/Core/Resource.h"

namespace Dark {

  class Texture : public Resource
  {
  public:
    Texture()
    {
      //DK_CORE_INFO("Texture()");
    }
    virtual ~Texture() = default;

    //virtual std::pair<Ref<ResourceID>, Ref<Resource>> LoadFromFile(const std::string& path)  = 0;

    virtual uint32_t GetWidth() const      = 0;
    virtual uint32_t GetHeight() const     = 0;
    virtual uint32_t GetRendererID() const = 0;

    virtual void Bind(uint32_t unit = 0) const = 0;
  };

  class Texture2D : public Texture
  {
  public:
    Texture2D()
    {
      //DK_CORE_INFO("Texture2D()");
    }

    Texture2D(const std::string& path)
    {
      this->m_ResID = CreateRef<ResourceID>(path);
      this->m_ResID->type = ResourceType::eTexture;
    }

    virtual ~Texture2D() = default;
    virtual std::pair<Ref<ResourceID>, Ref<Resource>> LoadFromFile(const std::string& path) override;
    virtual std::pair<Ref<ResourceID>, Ref<Resource>> LoadFromMemory(void* mem, size_t size) override;

    std::string GetType() { return std::string("Texture2D"); }

    virtual uint32_t GetWidth() const override { return 0; }
    virtual uint32_t GetHeight() const override { return 0; }
    virtual uint32_t GetRendererID() const override { return 0; }

    virtual void Bind(uint32_t unit = 0) const override {};
  };

} // namespace Dark