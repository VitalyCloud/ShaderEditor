//
//  ShaderPass.cpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 26.04.2022.
//

#include "Core/pch.h"
#include "ShaderPass.hpp"
#include "TextEditorPanel.hpp"
#include "EditorUniforms.hpp"

#include "ImGuiHelper.h"

namespace Editor {

ShaderPass::ShaderPass(const std::string& title) : m_Title(title) {
    m_Uniforms = Core::CreateRef<UniformBufferConteiner>();
}

ShaderPass::~ShaderPass() {
    
}

void ShaderPass::OnUpdate() {
    
    if(m_VertexPath != nullptr && m_FragmnetPath != nullptr) {
        if(m_VertexPath->GetStatus() == Core::Utils::FileStatus::FileChanged ||
           m_FragmnetPath->GetStatus() == Core::Utils::FileStatus::FileChanged)
            UpdateShader();
    }
    
    // Draw
    if(m_Shader != nullptr) {
        EditorUniforms::Get().Upload(m_Shader);
        m_Uniforms->UploadUniforms(m_Shader);
        
        m_Shader->Bind();
        for(int i=0; i<m_Meshes.size(); i++) {
            m_Meshes[i]->Draw(m_Shader);
        }
    }
}

void ShaderPass::UpdateShader() {
    if(m_VertexPath != nullptr && m_FragmnetPath != nullptr) {
        EN_INFO("Updating shader");
        auto vertexContent = m_VertexPath->Read();
        auto fragmentContent = m_FragmnetPath->Read();
        if(vertexContent.has_value() && fragmentContent.has_value()) {
            m_Shader = Core::CreateRef<OpenGL::Shader>(vertexContent.value(), fragmentContent.value());
        }
    }
}

void ShaderPass::SetVertexPath(const Core::Ref<Core::Utils::File>& vertexPath) {
    m_VertexPath = vertexPath;
    UpdateShader();
    TextEditorPanel::Get().AddBuffer(vertexPath);
}

void ShaderPass::SetFragmnetPath(const Core::Ref<Core::Utils::File>& fragmentPath) {
    m_FragmnetPath = fragmentPath;
    UpdateShader();
    TextEditorPanel::Get().AddBuffer(fragmentPath);
}

void ShaderPass::SetShaderPath(const Core::Ref<Core::Utils::File>& vertexPath,
                               const Core::Ref<Core::Utils::File>& fragmentPath) {
    m_VertexPath = vertexPath;
    m_FragmnetPath = fragmentPath;
    UpdateShader();
    TextEditorPanel::Get().AddBuffer(vertexPath);
    TextEditorPanel::Get().AddBuffer(fragmentPath);
}

void ShaderPassInspector::Draw() {
    if(m_Context == nullptr)
        return;
    
    std::string vertexPath = "";
    std::string fragmentPath = "";
    if(m_Context->m_VertexPath != nullptr)
        vertexPath = m_Context->m_VertexPath->GetPath();
    if(m_Context->m_FragmnetPath != nullptr)
        fragmentPath = m_Context->m_FragmnetPath->GetPath();
    
    if(ImGui::BeginTable("##ShaderPassProperties", 3)) {
        ImGui::TableSetupColumn("Property", ImGuiTableColumnFlags_WidthFixed, 100);
        ImGui::TableSetupColumn("Value");
        ImGui::TableSetupColumn("Action", ImGuiTableColumnFlags_WidthFixed, 10);
        
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::PushItemWidth(ImGui::GetColumnWidth());
        ImGui::Text("Title");
        ImGui::PopItemWidth();
        ImGui::TableSetColumnIndex(1);
        ImGui::PushItemWidth(ImGui::GetColumnWidth());
        ImGui::InputText("##ShaderPassTitle", &m_Context->m_Title);
        ImGui::PopItemWidth();
        
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::PushItemWidth(ImGui::GetColumnWidth());
        ImGui::Text("Vertex Path");
        ImGui::PopItemWidth();
        ImGui::TableSetColumnIndex(1);
        ImGui::PushItemWidth(ImGui::GetColumnWidth());
        ImGui::InputText("##ShaderPassVertexPath", &vertexPath, ImGuiInputTextFlags_ReadOnly);
        ImGui::PopItemWidth();
        ImGui::TableSetColumnIndex(2);
        if(ImGui::Button(":##VertexAtctionButton")) {
            m_ActionPopupSelector = ActionPopupSelector::Vertex;
            ImGui::OpenPopup("ShaderActionPopup");
        }
            
        
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::PushItemWidth(ImGui::GetColumnWidth());
        ImGui::Text("Fragment Path");
        ImGui::PopItemWidth();
        ImGui::TableSetColumnIndex(1);
        ImGui::PushItemWidth(ImGui::GetColumnWidth());
        ImGui::InputText("##ShaderPassFragmentPath", &fragmentPath, ImGuiInputTextFlags_ReadOnly);
        ImGui::PopItemWidth();
        ImGui::TableSetColumnIndex(2);
        if(ImGui::Button(":##FragmentAtctionButton")) {
            m_ActionPopupSelector = ActionPopupSelector::Fragment;
            ImGui::OpenPopup("ShaderActionPopup");
        }
        
        if(ImGui::BeginPopup("ShaderActionPopup")) {
            if(ImGui::Selectable("Open")) {
                auto path = Core::Utils::FileDialogs::OpenFile("");
                if(!path.empty()) {
                    auto file = Core::Utils::FileWatcher::Get().LoadFile(path);
                    if(m_ActionPopupSelector == ActionPopupSelector::Vertex)
                        m_Context->SetVertexPath(file);
                    if(m_ActionPopupSelector == ActionPopupSelector::Fragment)
                        m_Context->SetFragmnetPath(file);
                }
            }
            
            if(m_ActionPopupSelector == ActionPopupSelector::Vertex && m_Context->m_VertexPath != nullptr) {
                if(ImGui::Selectable("Show"))
                    TextEditorPanel::Get().AddBuffer(m_Context->m_VertexPath);
            }
            
            if(m_ActionPopupSelector == ActionPopupSelector::Fragment && m_Context->m_FragmnetPath != nullptr) {
                if(ImGui::Selectable("Show"))
                    TextEditorPanel::Get().AddBuffer(m_Context->m_FragmnetPath);
            }
            
            ImGui::EndPopup();
        }
        
        ImGui::EndTable();
        
        m_UniformView.SetContext(m_Context->m_Uniforms);
        m_UniformView.Draw();
        
    }
    
    
}

}
