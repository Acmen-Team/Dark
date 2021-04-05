#include "dkpch.h"

#include "RenderCommand.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Dark {

  RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;

}