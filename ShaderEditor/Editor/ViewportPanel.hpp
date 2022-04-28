//
//  ViewportPanel.hpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 21.04.2022.
//

#ifndef ViewportPanel_hpp
#define ViewportPanel_hpp

#include "imgui.h"
#include "glm/glm.hpp"
#include <functional>
#include "OpenGL/Framebuffer.hpp"
#include "Core/Core.hpp"

namespace Editor {

class ViewportPanel {
public:
    using ResizeCallbackFn = std::function<void(uint32_t width, uint32_t height)>;
public:
    ViewportPanel();
    void SetTexture(ImTextureID textureID);
    void SetFramebuffer(const Core::Ref<OpenGL::Framebuffer>& frambuffer);
    void SetResizeCallback(const ResizeCallbackFn& callback) { m_ResizeCallbackFn = callback; }
    void Draw(const char* title, bool* p_open = nullptr);
private:
    void DrawImage();
    void DrawMenuBar();
private:
    ImTextureID m_TextureID;
    glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
    bool m_SameAsViewport = false;
    Core::Ref<OpenGL::Framebuffer> m_Framebuffer = nullptr;
private:
    ResizeCallbackFn m_ResizeCallbackFn;
    float m_ResizeTime = 0.0f;
    bool m_Resized = false;
};

}

#endif /* ViewportPanel_hpp */
