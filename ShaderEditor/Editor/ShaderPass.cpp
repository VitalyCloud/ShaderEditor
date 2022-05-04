//
//  ShaderPass.cpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 01.05.2022.
//
#include "pch.h"
#include "ShaderPass.hpp"
#include "ImGuiHelper.hpp"

namespace Editor {

ShaderPass::ShaderPass(const std::string& title)
: m_Title(title) {
    
}

ShaderPass::~ShaderPass() {
    
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
}

void ShaderPass::SetFragmnetPath(const Core::Ref<Core::Utils::File>& fragmentPath) {
    m_FragmnetPath = fragmentPath;
    UpdateShader();
}

void ShaderPass::SetShaderPath(const Core::Ref<Core::Utils::File>& vertexPath,
                               const Core::Ref<Core::Utils::File>& fragmentPath) {
    m_VertexPath = vertexPath;
    m_FragmnetPath = fragmentPath;
    UpdateShader();
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
    }
}

void ShaderPassInspector::Draw()  {
    if(m_Context == nullptr) return;
    
    ImGui::InputText("Title##ShaderPassTitle", &m_Context->m_Title);
}


}
