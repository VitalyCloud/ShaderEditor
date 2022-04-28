
//  ViewportPanel.cpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 21.04.2022.
//
#include "Core/pch.h"
#include "ViewportPanel.hpp"

#include "Core/Core.hpp"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"

#include <map>

namespace Editor {

ViewportPanel::ViewportPanel() : m_ResizeCallbackFn([](uint32_t width, uint32_t height) {}){
    SetTexture(nullptr);
}

void ViewportPanel::SetTexture(ImTextureID textureID) { 
    m_TextureID = textureID;
}

void ViewportPanel::SetFramebuffer(const Core::Ref<OpenGL::Framebuffer>& frambuffer) {
    m_Framebuffer = frambuffer;
    if(m_Framebuffer != nullptr) {
        m_TextureID = (ImTextureID)(uintptr_t)m_Framebuffer->GetColorAttachmentRendererID();
    }
}

void ViewportPanel::Draw(const char *title, bool *p_open) { 
    
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    if(!ImGui::Begin(title, p_open, ImGuiWindowFlags_MenuBar |  ImGuiWindowFlags_NoScrollbar)) {
        ImGui::End();
        ImGui::PopStyleVar();
        return;
    }
    ImGui::PopStyleVar();
    
    
    
//    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
//    if (m_ViewportSize != *((glm::vec2*)&viewportPanelSize))
//    {
//        m_ResizeTime = Core::Input::GetTime();
//        uint32_t width = (uint32_t)viewportPanelSize.x;
//        uint32_t height = (uint32_t)viewportPanelSize.y;
//        m_ResizeCallbackFn(width, height);
//        m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
//    }
    
    if(m_SameAsViewport) {
        auto size = ImGui::GetContentRegionAvail();
        auto specification = m_Framebuffer->GetSpecification();
        if(specification.Width != size.x || specification.Height != size.y) {
            m_Framebuffer->Resize(size.x, size.y);
            m_ResizeTime = Core::Input::GetTime();
        }
    }
    
    DrawMenuBar();
    
    bool resizeElapsed = (Core::Input::GetTime() - m_ResizeTime) > 0.1f;
    if(resizeElapsed)
        DrawImage();
    
    ImGui::End();
    
}

void ViewportPanel::DrawImage() {
    float aspectRatio = m_Framebuffer->GetAspectRatio();
    auto viewportSize = ImGui::GetContentRegionAvail();
    auto size = ImVec2(viewportSize.x, viewportSize.x / aspectRatio);
    auto pos = (ImGui::GetContentRegionAvail() - size) * 0.5 + ImGui::GetWindowSize() - ImGui::GetContentRegionAvail();
    ImGui::SetCursorPos(pos);
    ImGui::Image(m_TextureID,
                 size,
                 ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
}

static const char* const s_Resolutions[] = {
    // 16x9
    "256x144",
    "320x180",
    "426x240",
    "640x360",
    "848x480",
    "854x480",
    "960x540",
    "1024x576",
    "1280x720",
    "1600x900",
    "1920x1080",
    "2560x1440",
    "3200x1800",
    "3840x2160",
    "5120x2880",
    "7680x4320", // 16
    // 4x3
    "160x120",
    "256x192",
    "320x240",
    "640x480",
    "800x600",
    "960x720",
    "1024x768",
    "1152x864",
    "1280x960",
    "1400x1050",
    "1440x1080",
    "1600x1200",
    "1856x1392",
    "1920x1440",
    "2048x1536",
    "2560x1920" // 32
};

const std::vector<int> resolutionMap = {
    144,
    180,
    240,
    360,
    480,
    480,
    540,
    576,
    720,
    900,
    1080,
    1440,
    1800,
    2160,
    2880,
    4320,
    
    // 4x3
    120,
    192,
    240,
    480,
    600,
    720,
    768,
    864,
    960,
    1050,
    1080,
    1200,
    1392,
    1440,
    1536,
    1920 // 32
};

void ViewportPanel::DrawMenuBar() {
    if(m_Framebuffer == nullptr) return;
    
    if(ImGui::BeginMenuBar()) {
        if(ImGui::BeginMenu("Resolution")) {
            if(ImGui::BeginMenu("16x9")) {
                for(int i=0; i<16; i++) {
                    
                    if(ImGui::MenuItem(s_Resolutions[i])) {
                        int height = resolutionMap[i];
                        int width = height * 16.0/9.0;
                        m_Framebuffer->Resize(width, height);
                        m_ResizeTime = Core::Input::GetTime();
                    }
                }
                ImGui::EndMenu();
            }
            if(ImGui::BeginMenu("4x3")) {
                for(int i=16; i<32; i++) {
                    if(ImGui::MenuItem(s_Resolutions[i])) {
                        int height = resolutionMap[i];
                        int width = height * 4.0/3.0;
                        m_Framebuffer->Resize(width, height);
                        m_ResizeTime = Core::Input::GetTime();
                    }
                }
                ImGui::EndMenu();
            }
            
            if(ImGui::MenuItem("Same as Viewport", "", m_SameAsViewport))
                m_SameAsViewport = !m_SameAsViewport;
            
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
}

}
