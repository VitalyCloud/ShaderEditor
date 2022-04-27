//
//  ShaderPass.cpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 26.04.2022.
//

#include "Core/pch.h"
#include "ShaderPass.hpp"
#include "TextEditorPanel.hpp"

#include "ImGuiHelper.h"

namespace Editor {

ShaderPass::ShaderPass(const std::string& title) : m_Title(title) {
    auto vertPath = Core::Utils::FileWatcher::Get().LoadFile("/Users/vitalycloud/Desktop/ShaderEditor/Resources/BasicShader/Basic.vert");
    auto fragPath = Core::Utils::FileWatcher::Get().LoadFile("/Users/vitalycloud/Desktop/ShaderEditor/Resources/BasicShader/Basic.frag");
    
    SetShaderPath(vertPath, fragPath);
    
    m_Uniforms = Core::CreateRef<UniformBufferConteiner>();
}

ShaderPass::~ShaderPass() {
    
}

void ShaderPass::OnUpdate() {
    
    if(m_VertexPath->GetStatus() == Core::Utils::FileStatus::FileChanged ||
       m_FragmnetPath->GetStatus() == Core::Utils::FileStatus::FileChanged)
        UpdateShader();
    
    m_Shader->Bind();
    m_Uniforms->UploadUniforms(m_Shader);
    
    // Draw
    if(m_Shader != nullptr) {
        m_Shader->Bind();
        for(auto& mesh : m_Meshes) {
            auto va = mesh->GetVertexArray();
            va->Bind();
            OpenGL::RenderCommand::DrawIndexed(va);
        }
    }
}

void ShaderPass::UpdateShader() {
    EN_INFO("Updating shader");
    auto vertexContent = m_VertexPath->Read();
    auto fragmentContent = m_FragmnetPath->Read();
    if(vertexContent.has_value() && fragmentContent.has_value()) {
        m_Shader = Core::CreateRef<OpenGL::Shader>(vertexContent.value(), fragmentContent.value());
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
    if(m_Context->m_VertexPath != nullptr)
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
            
            if(ImGui::Selectable("Show")) {
                if(m_ActionPopupSelector == ActionPopupSelector::Vertex)
                    TextEditorPanel::Get().AddBuffer(m_Context->m_VertexPath);
                if(m_ActionPopupSelector == ActionPopupSelector::Fragment)
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
