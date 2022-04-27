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

#include "EditorUniforms.hpp"

namespace Editor {

PipelinePanel::PipelinePanel() {
    m_Camera = Core::CreateRef<Renderer::OrthographicCamera>();
    auto triangle = Core::CreateRef<ShaderPass>("TriangleShaderPass");
    triangle->GetMeshes().push_back(Core::CreateRef<Mesh>("Triangle"));
    
    m_ShaderPasses.push_back(triangle);
}

PipelinePanel::~PipelinePanel() {
    
}

void PipelinePanel::OnUpdate() {
    for(auto& shaderPass: m_ShaderPasses) {
        EditorUniforms::Get().Upload(shaderPass->GetShader());
        shaderPass->OnUpdate();
    }
        
}

void PipelinePanel::Draw(const char* title, bool* p_open) {
    if (!ImGui::Begin(title, p_open)) {
        ImGui::End();
        return;
    }
    
    // Draw Camera
    bool isCameraSelected = InspectorPanel::Get().IsActive(&m_CameraInpector);
    if(ImGui::Selectable("Camera", isCameraSelected)) {
        m_CameraInpector.SetContext(m_Camera);
        InspectorPanel::Get().SetContext(&m_CameraInpector);
    }
    
    for(int i=0; i<m_ShaderPasses.size(); i++) {
        const char* title = m_ShaderPasses[i]->GetTitle().c_str();
        auto id = fmt::format("{0}##{1}", title, i);
        auto shaderPass = m_ShaderPasses[i];
        
        bool isShaderPassSelected = InspectorPanel::Get().IsActive(&m_ShaderPassInspector) && m_ShaderPassInspector.GetContext() == shaderPass;
        ImGuiTreeNodeFlags flags = (isShaderPassSelected) ? ImGuiTreeNodeFlags_Selected : 0;
        flags |= ImGuiTreeNodeFlags_OpenOnArrow;
        bool opened = ImGui::TreeNodeEx(id.c_str(), flags, "%s", title);
        if(ImGui::IsItemClicked()) {
            m_ShaderPassInspector.SetContext(shaderPass);
            InspectorPanel::Get().SetContext(&m_ShaderPassInspector);
        }
        
        if(opened) {
            for(auto& mesh: shaderPass->GetMeshes()) {
                bool isMeshSelected = InspectorPanel::Get().IsActive(&m_MeshInspector) && m_MeshInspector.GetContext() == mesh;
                if(ImGui::Selectable(mesh->GetTitle().c_str(), isMeshSelected)) {
                    m_MeshInspector.SetContext(mesh);
                    InspectorPanel::Get().SetContext(&m_MeshInspector);
                }
            }
            
            ImGui::TreePop();
        }
        
    }
    
    ImGui::End();
}


void CameraInspector::Draw() {
    if(m_Context == nullptr)
        return;
    
    if(ImGui::DragFloat3("Position##CameraPosition", &m_CameraPostion.x))
        m_Context->SetPostion(m_CameraPostion);
    if(ImGui::DragFloat("Rotation##CameraRotation", &rotation))
        m_Context->SetRotation(rotation);
};

}
