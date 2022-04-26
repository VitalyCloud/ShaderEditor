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

#include "glm/gtc/type_ptr.hpp"

namespace Editor {

PipelinePanel::PipelinePanel() {
    m_ShaderPassses.push_back(Core::CreateRef<ShaderPass>("Triangle"));
    m_Camera = Core::CreateRef<Renderer::OrthographicCamera>();
    m_UniformBuffer = Core::CreateRef<UniformBufferConteiner>();
    m_UniformBuffer->PushUniform(OpenGL::ShaderDataType::Mat4, "u_ViewProjection");
}

PipelinePanel::~PipelinePanel() {
    
}

void PipelinePanel::OnUpdate() {
    
    // Camera Update
    m_Camera->SetPostion(m_CameraPostion);
    m_UniformBuffer->SetUniformData("u_ViewProjection", glm::value_ptr(m_Camera->GetViewProjectionMatrix()));
    for(auto& shaderPass: m_ShaderPassses) {
        m_UniformBuffer->UploadUniforms(shaderPass->GetShader());
        shaderPass->OnUpdate();
    }
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
        
        
        ImGui::DragFloat3("##CameraPositionInput", &m_CameraPostion.x, 0.1, -10, 10);
        
        ImGui::Separator();
        
        if(ImGui::Button("Open Vertex")) {
            
        }
        
        if(ImGui::Button("Open Fragment")) {
            
        }
        
        m_SelectedShaderPasss->GetVA().Draw();
    }
    
    ImGui::End();
}


}
