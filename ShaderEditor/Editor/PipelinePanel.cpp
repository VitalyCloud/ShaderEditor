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
    auto vertPath = Core::Utils::FileWatcher::Get().LoadFile("/Users/vitalycloud/Desktop/ShaderEditor/Resources/BasicShader/Basic.vert");
    auto fragPath = Core::Utils::FileWatcher::Get().LoadFile("/Users/vitalycloud/Desktop/ShaderEditor/Resources/BasicShader/Basic.frag");
    triangle->GetMeshes().push_back(Core::CreateRef<Mesh>(DefaultMesh::Triangle));
    triangle->SetShaderPath(vertPath, fragPath);
    
    m_ShaderPasses.push_back(triangle);
}

PipelinePanel::~PipelinePanel() {
    
}

void PipelinePanel::OnUpdate() {
    EditorUniforms::Get().SetViewProjection(m_Camera->GetViewProjectionMatrix());
    for(auto& shaderPass: m_ShaderPasses) {
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
    
    ImGui::Separator();
    
    for(int i=0; i<m_ShaderPasses.size(); i++) {
        ImGui::PushID(i);
        const char* title = m_ShaderPasses[i]->GetTitle().c_str();
        auto id = fmt::format("{0}##{1}", title, i);
        auto shaderPass = m_ShaderPasses[i];
        
        bool isShaderPassSelected = InspectorPanel::Get().IsActive(&m_ShaderPassInspector) && m_ShaderPassInspector.GetContext() == shaderPass;
        ImGuiTreeNodeFlags flags = (isShaderPassSelected) ? ImGuiTreeNodeFlags_Selected : 0;
        flags |= ImGuiTreeNodeFlags_OpenOnArrow;
        bool opened = ImGui::TreeNodeEx(id.c_str(), flags, "%s", title);
        if(ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
            m_ShaderPassInspector.SetContext(shaderPass);
            InspectorPanel::Get().SetContext(&m_ShaderPassInspector);
        }
        if(ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
            ImGui::OpenPopup("ShaderPassPopup");
        }
        
        
        if(ImGui::BeginPopup("ShaderPassPopup")) {
            if(m_ShaderPasses.size() > 1) {
                if(i > 0) {
                    if(ImGui::Selectable("Move Up")) {
                        std::swap(*(m_ShaderPasses.begin() + i), *(m_ShaderPasses.begin() + i - 1));
                    }
                }
                
                if(i < m_ShaderPasses.size() - 1) {
                    if(ImGui::Selectable("Move Down")) {
                        std::swap(*(m_ShaderPasses.begin() + i), *(m_ShaderPasses.begin() + i + 1));
                    }
                }
                ImGui::Separator();
            }
            
            
            
            if(ImGui::BeginMenu("Add Mesh")) {
                if(ImGui::Selectable("Empty")) {
                    auto title = fmt::format("Mesh#{0}", m_ShaderPasses[i]->GetMeshes().size());
                    m_ShaderPasses[i]->GetMeshes().push_back(Core::CreateRef<Mesh>(title));
                }
                
                if(ImGui::Selectable("Triangle")) {
                    m_ShaderPasses[i]->GetMeshes().push_back(Core::CreateRef<Mesh>(DefaultMesh::Triangle));
                }
                
                if(ImGui::Selectable("Squad")) {
                    m_ShaderPasses[i]->GetMeshes().push_back(Core::CreateRef<Mesh>(DefaultMesh::Squad));
                }
                
                ImGui::EndMenu();
            }
            
            if(ImGui::Selectable("Delete")) {
                m_ShaderPasses.erase(m_ShaderPasses.begin() + i);
            }
            
            ImGui::EndPopup();
        }
        
        if(opened) {
            for(int j=0; j<m_ShaderPasses[i]->GetMeshes().size(); j++) {
                auto& mesh = m_ShaderPasses[i]->GetMeshes()[j];
                ImGui::PushID(j);
                bool isMeshSelected = InspectorPanel::Get().IsActive(&m_MeshInspector) && m_MeshInspector.GetContext() == mesh;
                if(ImGui::Selectable(mesh->GetTitle().c_str(), isMeshSelected)) {
                    m_MeshInspector.SetContext(mesh);
                    InspectorPanel::Get().SetContext(&m_MeshInspector);
                }
                if(ImGui::IsItemClicked(ImGuiMouseButton_Right) && ImGui::IsItemFocused())
                    ImGui::OpenPopup("MeshPopup");
                if(ImGui::BeginPopup("MeshPopup")) {
                    if(ImGui::Selectable("Delete"))
                        m_ShaderPasses[i]->GetMeshes().erase(m_ShaderPasses[i]->GetMeshes().begin() + j);
                    ImGui::EndPopup();
                }
                ImGui::PopID();
            }
            ImGui::TreePop();
        }
        ImGui::PopID();
    }
    
    
    
    ImGui::BeginChild("Pipeline The Rest");
    if(ImGui::IsMouseClicked(ImGuiMouseButton_Right) && ImGui::IsWindowHovered()) {
        ImGui::OpenPopup("PipelinePopup");
    }
    if(ImGui::IsMouseClicked(ImGuiMouseButton_Left) && ImGui::IsWindowHovered()) {
        InspectorPanel::Get().SetContext(nullptr);
    }
    
    if(ImGui::BeginPopup("PipelinePopup")) {
        if(ImGui::Selectable("Add ShaderPass")) {
            auto title = fmt::format("ShaderPass#{0}", m_ShaderPasses.size());
            m_ShaderPasses.push_back(Core::CreateRef<ShaderPass>(title));
        }
        ImGui::EndPopup();
    }
    
    ImGui::EndChild();
    
   
    
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
