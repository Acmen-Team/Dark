#pragma once

#include "Dark/Renderer/Framebuffer.h"

namespace Dark {

  class OpenGLFramebuffer: public Framebuffer
  {
  public:
	OpenGLFramebuffer(const FramebufferSpecification& spec);
	virtual ~OpenGLFramebuffer();

	void Invalidate();

	virtual void Bind() override;
	virtual void UnBind() override;

	virtual uint32_t GetColorAttachmentRendererID() const override { return m_ColorAttachment; }

	virtual const FramebufferSpecification& GetSpecification() const override { return m_Specification; }
  protected:

  private:
	uint32_t m_RendererID;
	uint32_t m_ColorAttachment, m_DepthAttachment;
	FramebufferSpecification m_Specification;
  };

}