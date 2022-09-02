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
    virtual ~Texture2D() = default;
    virtual std::pair<Ref<ResourceID>, Ref<Resource>> LoadFromFile(const std::string& path) override;

    std::string GetType() { return std::string("Texture2D"); }

    virtual uint32_t GetWidth() const { return 0; }
    virtual uint32_t GetHeight() const { return 0; }
    virtual uint32_t GetRendererID() const { return 0; }

    virtual void Bind(uint32_t unit = 0) const {};
  };
  //{
  //public:
  //  virtual ~Texture2D() = default;
  //  static Ref<Texture2D> Create(const std::string& path);

  //  virtual std::tuple<ResourceID*, Resource*> LoadFromFile(const std::string& path);

  //  //static Ref<Texture2D> LoadFromFile(const std::string& path);
  //};

} // namespace Dark