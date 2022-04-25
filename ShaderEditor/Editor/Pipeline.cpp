//
//  Pipeline.cpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 25.04.2022.
//

#include "Core/pch.h"
#include "Pipeline.hpp"

#include "imgui.h"
#include "ImGuiHelper.h"

namespace Editor {

Pipeline::Pipeline() {
    m_ShaderPassses.push_back(Core::CreateRef<ShaderPass>("1"));
    m_ShaderPassses.push_back(Core::CreateRef<ShaderPass>("2"));
}

Pipeline::~Pipeline() {
    
}

void Pipeline::Draw(const char* title, bool* p_open) {
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
    
    DrawInspector();
   
    ImGui::End();
}

void Pipeline::DrawInspector() {
    ImGui::Begin("Inspector");
    
    if(m_SelectedShaderPasss != nullptr) {
        ImGui::Text("Title"); ImGui::SameLine();
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
        ImGui::InputText("##inputTitle", &m_SelectedShaderPasss->GetTitle());
        ImGui::PopItemWidth();
        
        m_SelectedShaderPasss->GetVA().Draw();
    }
    
    
    ImGui::End();
}


VertexArray::VertexArray() {
    m_VertexBuffers.push_back(Core::CreateRef<VertexBuffer>());
    m_Indicies.resize(1, 0);
}

VertexArray::~VertexArray() {
    
}
                             

void VertexArray::Draw() {
    ImGui::Separator();
    for(int i=0; i<m_VertexBuffers.size(); i++) {
        auto title = fmt::format("Buffer #{0}", i);
        bool close = true;
        
        bool open = ImGui::CollapsingHeader(title.c_str(), &close);
        if(ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
            ImGui::OpenPopup("BufferContext");
        }
        
        if(open) {
            ImGui::PushID(i);
            m_VertexView.SetContext(m_VertexBuffers[i]);
            m_VertexView.Draw();
            ImGui::PopID();
        }
        
        if(!close && m_VertexBuffers.size() > 1) {
            m_VertexBuffers.erase(m_VertexBuffers.begin() + i);
        }
    }
   
    if(ImGui::BeginPopup("BufferContext")) {
        if(ImGui::Selectable("Add Buffer")) {
            m_VertexBuffers.push_back(Core::CreateRef<VertexBuffer>());
        }
        ImGui::EndPopup();
    }
    
    if(ImGui::CollapsingHeader("Index Buffer")) {
        m_IndexView.Draw(m_Indicies);
    }
}

}
