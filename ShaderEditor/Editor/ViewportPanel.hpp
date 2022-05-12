//
//  ViewportPanel.hpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 01.05.2022.
//

#ifndef ViewportPanel_hpp
#define ViewportPanel_hpp

#include "Core/Memory.hpp"
#include "OpenGL/Framebuffer.hpp"

#include "imgui.h"
#include "glm/glm.hpp"

namespace Editor {

class ViewportPanel {
public:
    ViewportPanel();
    
    void SetFramebuffer(const Core::Ref<OpenGL::Framebuffer>& framebuffer);
    
    void Draw(const char* title, bool* p_open = nullptr);
    
private:
    void DrawImage();
    void DrawMenuBar();
    void Resize(uint32_t width, uint32_t height);
    
private:
    ImTextureID m_TextureID;
    Core::Ref<OpenGL::Framebuffer> m_Framebuffer = nullptr;
    
    // Image Settings
    bool m_ImageCentered = true;
    
    // Resizing
    bool m_SameSizeAsViewport = true;
    float m_ResizeTime = 0.0f;
    bool m_Resized = false;
};

}

#endif /* ViewportPanel_hpp */
