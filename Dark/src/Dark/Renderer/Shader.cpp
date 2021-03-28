#include "dkpch.h"

#include "Shader.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Dark {

  Shader* Shader::Create(const std::string& filePath)
  {
	switch (Renderer::GetAPI())
	{
	  case RendererAPI::API::None:  DK_CORE_ASSERT(false, "RendererAPI::None is not supported!"); return nullptr;
	  case RendererAPI::API::OpenGL: return new OpenGLShader(filePath);
	}

	DK_CORE_ASSERT(false, "Unkonw RendererAPI!");
	return nullptr;
  }

  Shader* Shader::Create(const char* vertexPath, const char* fragmentPath)
  {
	switch (Renderer::GetAPI())
	{
	  case RendererAPI::API::None:  DK_CORE_ASSERT(false, "RendererAPI::None is not supported!"); return nullptr;
	  case RendererAPI::API::OpenGL: return new OpenGLShader(vertexPath, fragmentPath);
	}

	DK_CORE_ASSERT(false, "Unkonw RendererAPI!");
	return nullptr;
  }

  Shader* Shader::Create(const std::string& vertexSource, const std::string& fragmentSource)
  {
	switch (Renderer::GetAPI())
	{
	  case RendererAPI::API::None:  DK_CORE_ASSERT(false, "RendererAPI::None is not supported!"); return nullptr;
	  case RendererAPI::API::OpenGL: return new OpenGLShader(vertexSource, fragmentSource);
	}

	DK_CORE_ASSERT(false, "Unkonw RendererAPI!");
	return nullptr;
  }

}