//
//  ShaderPass.cpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 01.05.2022.
//
#include "pch.h"
#include "ShaderPass.hpp"
#include "ImGuiHelper.hpp"
#include "TextEditorPanel.hpp"
#include "PropertyTable.hpp"
#include "EditorUniforms.hpp"
#include "MessagePanel.hpp"

namespace Editor {

ShaderPass::ShaderPass(const std::string& title)
: m_Title(title) {
    m_Uniforms = Core::CreateRef<UniformBufferContainer>();
}

ShaderPass::~ShaderPass() {
    MessagePanel::Get().DeleteMessage(this);
}

void ShaderPass::OnUpdate() {
    if(m_AutoCompile) {
        if(m_VertexPath != nullptr && m_FragmnetPath != nullptr) {
            if(m_VertexPath->GetStatus() == Core::Utils::FileStatus::FileChanged ||
               m_FragmnetPath->GetStatus() == Core::Utils::FileStatus::FileChanged)
                UpdateShader();
        }
    }
    
    if(m_Shader != nullptr) {
        EditorUniforms::Get().Upload(m_Shader);
        m_Uniforms->UploadUniforms(m_Shader);
        m_Shader->Bind();
        for(auto& object: m_ShaderPassObjects) {
            object->OnUpdate(m_Shader);
        }
        m_Shader->Unbind();
    }
}

void ShaderPass::SetVertexPath(const Core::Ref<Core::Utils::File>& vertexPath) {
    m_VertexPath = vertexPath;
    UpdateShader();
    if(vertexPath != nullptr)
        TextEditorPanel::Get().OpenFile(vertexPath);
}

void ShaderPass::SetFragmnetPath(const Core::Ref<Core::Utils::File>& fragmentPath) {
    m_FragmnetPath = fragmentPath;
    UpdateShader();
    if(fragmentPath != nullptr)
        TextEditorPanel::Get().OpenFile(fragmentPath);
}

void ShaderPass::SetShaderPath(const Core::Ref<Core::Utils::File>& vertexPath,
                               const Core::Ref<Core::Utils::File>& fragmentPath) {
    m_VertexPath = vertexPath;
    m_FragmnetPath = fragmentPath;
    UpdateShader();
    if(vertexPath != nullptr)
        TextEditorPanel::Get().OpenFile(vertexPath);
    if(fragmentPath != nullptr)
        TextEditorPanel::Get().OpenFile(fragmentPath);
}

void ShaderPass::UpdateShader() {
    if(m_VertexPath != nullptr && m_FragmnetPath != nullptr) {
        EN_INFO("Updating shader {0}", m_Title);
        auto vertexContent = m_VertexPath->Read();
        auto fragmentContent = m_FragmnetPath->Read();
        if(!vertexContent.has_value()) {
            EN_ERROR("Error while reading vertex shader file {0}", m_VertexPath->GetPath().string());
            return;
        }
        if(!fragmentContent.has_value()) {
            EN_ERROR("Error while reading fragment shader file {0}", m_FragmnetPath->GetPath().string());
            return;
        }
        m_Shader = Core::CreateRef<OpenGL::Shader>(vertexContent.value(), fragmentContent.value());
        if(m_Shader->HasError())
            MessagePanel::Get().SetMessage(this, m_Shader->GetErrorMessage());
        else
            MessagePanel::Get().DeleteMessage(this);
    }
}

void ShaderPassInspector::Draw()  {
    if(m_Context == nullptr) return;
    
    ImGui::PushID("ShaderPassInspector");
    
    ImGui::Text("ShaderPass");
    ImGui::Separator();
    if(PropertyTable::Begin("ShaderPassInspectorProperties")) {
        PropertyTable::Text("Title", m_Context->m_Title);
        
        ImGui::Separator();
        {
            // Vertex Path
            bool action = false;
            std::string path = m_Context->m_VertexPath != nullptr ? m_Context->m_VertexPath->GetPath().string() : "";
            PropertyTable::ReadOnlyText("Vertex Path", path, &action);
            if(action) {
                std::string newPath = Core::Utils::FileDialogs::OpenFile("");
                if(!newPath.empty()) {
                    m_Context->SetVertexPath(Core::Utils::FileWatcher::Get().LoadFile(newPath));
                }
            }
        }
        {
            // Fragmnent Path
            bool action = false;
            std::string path = m_Context->m_FragmnetPath != nullptr ? m_Context->m_FragmnetPath->GetPath().string() : "";
            PropertyTable::ReadOnlyText("Fragment Path", path, &action);
            if(action) {
                std::string newPath = Core::Utils::FileDialogs::OpenFile("");
                if(!newPath.empty()) {
                    m_Context->SetFragmnetPath(Core::Utils::FileWatcher::Get().LoadFile(newPath));
                }
            }
        }
        
        ImGui::Separator();
        PropertyTable::Checkbox("Auto compilation", m_Context->m_AutoCompile);
        
        PropertyTable::End();
    }
    
    if(ImGui::CollapsingHeader("Uniforms")) {
        m_UniformView.SetContext(m_Context->m_Uniforms);
        m_UniformView.Draw();
    }
    
    ImGui::Separator();
    
    ImGui::PopID();
}


}
