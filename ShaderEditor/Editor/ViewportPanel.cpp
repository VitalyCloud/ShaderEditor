
//  ViewportPanel.cpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 21.04.2022.
//
#include "Core/pch.h"
#include "ViewportPanel.hpp"

#include "Core/Core.hpp"

namespace Editor {

ViewportPanel::ViewportPanel() : m_ResizeCallbackFn([](uint32_t width, uint32_t height) {}){
    SetTexture(nullptr);
}

void ViewportPanel::SetTexture(ImTextureID textureID) { 
    m_TextureID = textureID;
}

void ViewportPanel::Draw(const char *title, bool *p_open) { 
    
    if(!ImGui::Begin(title, p_open)) {
        ImGui::End();
        return;
    }
    
    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
    if (m_ViewportSize != *((glm::vec2*)&viewportPanelSize))
    {
        m_ResizeTime = Engine::Input::GetTime();
        uint32_t width = (uint32_t)viewportPanelSize.x;
        uint32_t height = (uint32_t)viewportPanelSize.y;
        m_ResizeCallbackFn(width, height);
        m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
    }
    
    bool resizeElapsed = (Engine::Input::GetTime() - m_ResizeTime) > 0.1f;
    if(resizeElapsed)
        DrawImage();
    
    ImGui::End();
}

void ViewportPanel::DrawImage() { 
    ImGui::Image(m_TextureID,
                 ImVec2{ m_ViewportSize.x, m_ViewportSize.y },
                 ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
}

}
