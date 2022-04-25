//
//  Pipeline.cpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 25.04.2022.
//

#include "Core/pch.h"
#include "PipelinePanel.hpp"

#include "imgui.h"
#include "ImGuiHelper.h"

namespace Editor {

PipelinePanel::PipelinePanel() {
    m_ShaderPassses.push_back(Core::CreateRef<ShaderPass>("Triangle"));
}

PipelinePanel::~PipelinePanel() {
    
}

void PipelinePanel::OnUpdate() {
    for(auto& shaderPass: m_ShaderPassses)
        shaderPass->OnUpdate();
}

void PipelinePanel::Draw(const char* title, bool* p_open) {
    if (!ImGui::Begin(title, p_open)) {
        ImGui::End();
        return;
    }
    
    for(int i=0; i<m_ShaderPassses.size(); i++) {
        auto& shaderPass = m_ShaderPassses[i];
        auto title = fmt::format("{0}##{1}", shaderPass->GetTitle(), i);
        ImGui::Selectable(title.c_str(), m_SelectedShaderPasss == shaderPass);
        if (ImGui::IsItemClicked()) {
            m_SelectedShaderPasss = shaderPass;
        }
    }
    ImGui::End();
}

void PipelinePanel::DrawInspector(const char* title, bool* p_open) {
    if (!ImGui::Begin(title, p_open)) {
        ImGui::End();
        return;
    }
    
    if(m_SelectedShaderPasss != nullptr) {
        ImGui::Text("Title"); ImGui::SameLine();
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
        ImGui::InputText("##inputTitle", &m_SelectedShaderPasss->GetTitle());
        ImGui::PopItemWidth();
        
        m_SelectedShaderPasss->GetVA().Draw();
    }
    
    ImGui::End();
}


}
