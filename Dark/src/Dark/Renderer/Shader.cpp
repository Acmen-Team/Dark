#include "dkpch.h"

#include "Shader.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Dark {

  Ref<Shader> Shader::Create(const std::string& filePath)
  {
	switch (Renderer::GetAPI())
	{
	  case RendererAPI::API::None:  DK_CORE_ASSERT(false, "RendererAPI::None is not supported!"); return nullptr;
	  case RendererAPI::API::OpenGL: return std::make_shared<OpenGLShader>(filePath);
	}

	DK_CORE_ASSERT(false, "Unkonw RendererAPI!");
	return nullptr;
  }

  Ref<Shader> Shader::Create(const char* vertexPath, const char* fragmentPath)
  {
	switch (Renderer::GetAPI())
	{
	  case RendererAPI::API::None:  DK_CORE_ASSERT(false, "RendererAPI::None is not supported!"); return nullptr;
	  case RendererAPI::API::OpenGL: return std::make_shared<OpenGLShader>(vertexPath, fragmentPath);
	}

	DK_CORE_ASSERT(false, "Unkonw RendererAPI!");
	return nullptr;
  }

  Ref<Shader> Shader::Create(const std::string& name,const std::string& vertexSource, const std::string& fragmentSource)
  {
	switch (Renderer::GetAPI())
	{
	  case RendererAPI::API::None:  DK_CORE_ASSERT(false, "RendererAPI::None is not supported!"); return nullptr;
	  case RendererAPI::API::OpenGL: return std::make_shared<OpenGLShader>(name, vertexSource, fragmentSource);
	}

	DK_CORE_ASSERT(false, "Unkonw RendererAPI!");
	return nullptr;
  }

  void ShaderLibrary::Add(const Ref<Shader>& shader)
  {
	auto& name = shader->GetName();
	Add(name, shader);
  }

  void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader)
  {
	DK_CORE_ASSERT(!Exists(name), "Shader already exists!");
	m_Shaders[name] = shader;
  }

  Ref<Shader> ShaderLibrary::Load(const std::string& filepath)
  {
	auto shader = Shader::Create(filepath);
	Add(shader);
	return shader;
  }

  Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filepath)
  {
	auto shader = Shader::Create(filepath);
	Add(name, shader);
	return shader;
  }

  Ref<Shader> ShaderLibrary::Get(const std::string& name)
  {
	DK_CORE_ASSERT(Exists(name), "Shader not found!");
	return m_Shaders[name];
  }

  bool ShaderLibrary::Exists(const std::string& name) const
  {
	return m_Shaders.find(name) != m_Shaders.end();
  }

}