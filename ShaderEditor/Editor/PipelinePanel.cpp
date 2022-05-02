//
//  PipelinePanel.cpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 01.05.2022.
//
#include "pch.h"
#include "PipelinePanel.hpp"
#include "InspectorPanel.hpp"

#include "Mesh.hpp"

#include "imgui.h"

namespace Editor {

PipelinePanel::PipelinePanel() {
    
}

PipelinePanel::~PipelinePanel() {
    
}

void PipelinePanel::Draw(const char* title, bool* p_open) {
    if (!ImGui::Begin(title, p_open) || m_Context == nullptr) {
        ImGui::End();
        return;
    }
    
    for(int i=0; i<m_Context->GetShaderPasses().size(); i++) {
        ImGui::PushID(i);
        
        auto shaderPass = m_Context->GetShaderPasses()[i];
        
        const char* title = shaderPass->GetTitle().c_str();
        auto id = fmt::format("{0}##{1}", title, i);
        
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
            if(m_Context->GetShaderPasses().size() > 1) {
                if(i > 0) {
                    if(ImGui::Selectable("Move Up")) {
                        std::swap(*(m_Context->GetShaderPasses().begin() + i), *(m_Context->GetShaderPasses().begin() + i - 1));
                    }
                }
                
                if(i < m_Context->GetShaderPasses().size() - 1) {
                    if(ImGui::Selectable("Move Down")) {
                        std::swap(*(m_Context->GetShaderPasses().begin() + i), *(m_Context->GetShaderPasses().begin() + i + 1));
                    }
                }
                ImGui::Separator();
            }
            
            
            
            if(ImGui::BeginMenu("Add Mesh")) {
                if(ImGui::Selectable("Empty")) {
                    auto title = fmt::format("Mesh#{0}", shaderPass->GetShaderPassObjects().size());
                    
                    auto mesh = Core::CreateRef<Mesh>(title);
                    shaderPass->GetShaderPassObjects().push_back(mesh);
                }
                
                if(ImGui::Selectable("Traingle")) {
                    auto mesh = Core::CreateRef<Mesh>(DefaultMesh::Triangle);
                    shaderPass->GetShaderPassObjects().push_back(mesh);
                }
                
                if(ImGui::Selectable("Quad")) {
                    auto mesh = Core::CreateRef<Mesh>(DefaultMesh::Quad);
                    shaderPass->GetShaderPassObjects().push_back(mesh);
                }
                ImGui::EndMenu();
            }
            
            if(ImGui::Selectable("Delete")) {
                if(m_ShaderPassInspector.GetContext() == shaderPass)
                    m_ShaderPassInspector.SetContext(nullptr);
                
                m_Context->GetShaderPasses().erase(m_Context->GetShaderPasses().begin() + i);
                
            }
            
            ImGui::EndPopup();
        }
        
        if(opened) {
            for(int j=0; j<shaderPass->GetShaderPassObjects().size(); j++) {
                auto& object = shaderPass->GetShaderPassObjects()[j];
                ImGui::PushID(j);
                
                if(Mesh* mesh = dynamic_cast<Mesh*>(object.get()); mesh != nullptr) {
                    bool isMeshSelected = InspectorPanel::Get().IsActive(&m_MeshInspector) && m_MeshInspector.GetContext() == mesh;
                    if(ImGui::Selectable(mesh->GetTitle().c_str(), isMeshSelected)) {
                        m_MeshInspector.SetContext(mesh);
                        InspectorPanel::Get().SetContext(&m_MeshInspector);
                    }
                    if(ImGui::IsItemClicked(ImGuiMouseButton_Right) && ImGui::IsItemFocused())
                        ImGui::OpenPopup("ObjectPopup");
                    
                    if(ImGui::BeginPopup("ObjectPopup")) {
                        if(ImGui::Selectable("Delete")) {
                            if(m_MeshInspector.GetContext() == mesh)
                                m_MeshInspector.SetContext(nullptr);
                            shaderPass->GetShaderPassObjects().erase(shaderPass->GetShaderPassObjects().begin() + j);
                        }
                        ImGui::EndPopup();
                    }
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
            auto title = fmt::format("ShaderPass#{0}", m_Context->GetShaderPasses().size());
            m_Context->GetShaderPasses().push_back(Core::CreateRef<ShaderPass>(title));
        }
        ImGui::EndPopup();
    }
    
    ImGui::EndChild();
    
    ImGui::End();
}

}
