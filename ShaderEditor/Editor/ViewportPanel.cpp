//
//  ViewportPanel.cpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 01.05.2022.
//

#include "ViewportPanel.hpp"
#include "Core/Input/Input.hpp"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"

namespace Editor {


ViewportPanel::ViewportPanel() {
    SetFramebuffer(nullptr);
}

void ViewportPanel::SetFramebuffer(const Core::Ref<OpenGL::Framebuffer>& framebuffer) {
    m_Framebuffer = framebuffer;
    m_TextureID = 0;
    if(m_Framebuffer != nullptr) {
        m_TextureID = (ImTextureID)(uintptr_t)m_Framebuffer->GetColorAttachmentRendererID();
    }
}

void ViewportPanel::Draw(const char* title, bool* p_open) {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    static ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar |  ImGuiWindowFlags_NoScrollbar;
    if(!ImGui::Begin(title, p_open, windowFlags)) {
        ImGui::End();
        ImGui::PopStyleVar();
        return;
    }
    ImGui::PopStyleVar();
    
    if(m_SameSizeAsViewport) {
        auto size = ImGui::GetContentRegionAvail();
        auto specification = m_Framebuffer->GetSpecification();
        if(specification.Width != size.x || specification.Height != size.y) {
            m_Framebuffer->Resize(size.x, size.y);
            m_ResizeTime = Core::Input::GetTime();
            m_Resized = true;
        }
    }
    
    DrawMenuBar();
    
    bool resizeElapsed = (Core::Input::GetTime() - m_ResizeTime) > 0.1f;
    if(resizeElapsed) DrawImage();
    
    ImGui::End();
}

void ViewportPanel::DrawImage() {
    float aspectRatio = m_Framebuffer->GetAspectRatio();
    auto viewportSize = ImGui::GetContentRegionAvail();
    auto imageSize = ImVec2(viewportSize.x, viewportSize.x / aspectRatio);
    if(m_ImageCentered) {
        auto imagePos = (ImGui::GetContentRegionAvail() - imageSize) * 0.5 + ImGui::GetWindowSize() - ImGui::GetContentRegionAvail();
        ImGui::SetCursorPos(imagePos);
    }
    ImGui::Image(m_TextureID, imageSize, ImVec2(0,1), ImVec2(1,0));
}

void ViewportPanel::DrawMenuBar() {
    
}


}
