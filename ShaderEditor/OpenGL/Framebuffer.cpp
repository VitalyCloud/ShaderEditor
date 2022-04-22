//
//  Framebuffer.cpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 21.04.2022.
//

#include "Framebuffer.hpp"

#include "glad/glad.h"
#include "Core/Assert.h"


namespace OpenGL {

Framebuffer::Framebuffer(const FramebufferSpecification &spec)
    : m_Specification(spec){
        Invalidate();
}

Framebuffer::~Framebuffer() {
    glDeleteFramebuffers(1, &m_RendererID);
    glDeleteTextures(1, &m_ColorAttachment);
    glDeleteRenderbuffers(1, &m_RenderBuffer);
}

void Framebuffer::Invalidate() {
    if (m_RendererID)
    {
        glDeleteFramebuffers(1, &m_RendererID);
        glDeleteTextures(1, &m_ColorAttachment);
        glDeleteRenderbuffers(1, &m_RenderBuffer);
    }
    
    glGenFramebuffers(1, &m_RendererID);
    glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

    // Color
    glGenTextures(1, &m_ColorAttachment);
    glBindTexture(GL_TEXTURE_2D, m_ColorAttachment);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Specification.Width, m_Specification.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    // Attach texture to the buffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D, m_ColorAttachment, 0);
    
    // Gen Render Buffer
    glGenRenderbuffers(1, &m_RenderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_Specification.Width, m_Specification.Height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                              GL_RENDERBUFFER, m_RenderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    
    
    // Check status
    EN_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is not completed!")
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::Bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
    glViewport(0, 0, m_Specification.Width, m_Specification.Height);
}

void Framebuffer::Unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::Resize(unsigned int width, unsigned int height) {
    m_Specification.Width = width;
    m_Specification.Height = height;
    Invalidate();
}


}
