#pragma once

#include <glm/glm.hpp>

namespace Dark {

  class Shader
  {
  public:
	virtual ~Shader() = default;

	virtual void use() const = 0;

	virtual const std::string& GetName() const = 0;

	static Ref<Shader> Create(const std::string& filePath);
	static Ref<Shader> Create(const char* vertexPath, const char* fragmentPath);
	static Ref<Shader> Create(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource);
  };

  class ShaderLibrary
  {
  public:
	void Add(const Ref<Shader>& shader);
	void Add(const std::string& name, const Ref<Shader>& shader);
	Ref<Shader> Load(const std::string& filepath);
	Ref<Shader> Load(const std::string& name, const std::string& filepath);

	Ref<Shader> Get(const std::string& name);

	bool Exists(const std::string& name) const;
  private:
	std::unordered_map<std::string, Ref<Shader>> m_Shaders;
  };

}