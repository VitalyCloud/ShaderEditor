//
//  Framebuffer.hpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 21.04.2022.
//

#ifndef Framebuffer_hpp
#define Framebuffer_hpp

#include <stdint.h>

namespace OpenGL {

struct FramebufferSpecification
{
    uint32_t Width, Height;
};

class Framebuffer {
public:
    Framebuffer(const FramebufferSpecification& spec);
    virtual ~Framebuffer();
    
    void Invalidate();
    
    void Bind();
    void Unbind();
    
    void Resize(uint32_t width, uint32_t height);
    
    unsigned int  GetColorAttachmentRendererID() { return m_ColorAttachment; }
    const FramebufferSpecification& GetSpecification() { return m_Specification; }
    
private:
    unsigned int m_RendererID;
    unsigned int m_ColorAttachment = 0;
    unsigned int m_RenderBuffer = 0;
    
    FramebufferSpecification m_Specification;
};

}

#endif /* Framebuffer_hpp */
