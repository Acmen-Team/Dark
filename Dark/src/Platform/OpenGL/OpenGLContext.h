#pragma once

#include "Dark/Renderer/RendererContext.h"

struct GLFWwindow;

namespace Dark {

  class OpenGLContext : public RenderContext
  {
  public:
	OpenGLContext(GLFWwindow* windowHanle);

	virtual void Init() override;
	virtual void SwapBuffers() override;
  private:
	GLFWwindow* m_WindowHandle;
  };

}