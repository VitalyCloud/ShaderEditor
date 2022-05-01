//
//  Mesh.cpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 01.05.2022.
//
#include "pch.h"
#include "Mesh.hpp"
#include "ImGuiHelper.hpp"
#include "OpenGL/RenderCommand.hpp"

namespace Editor {

Mesh::Mesh(const std::string& title)
: m_Title(title){
    m_VertexArrayContainer = Core::CreateRef<VertexArrayContainer>();
    m_VertexArrayContainer->AddVertexBuffer();
}

Mesh::~Mesh() {

}

void Mesh::OnUpdate(const Core::Ref<OpenGL::Shader>& shader) {
    auto va = m_VertexArrayContainer->GetVA();
    if(va != nullptr) {
        shader->Bind();
        va->Bind();
        OpenGL::RenderCommand::DrawIndexed(va);
    }
}

void MeshInspector::Draw() {
    if(m_Context == nullptr) return;
    
    ImGui::InputText("Title##MeshTitle", &m_Context->m_Title);
    m_VertexArrayView.SetContext(m_Context->m_VertexArrayContainer);
    m_VertexArrayView.Draw();
}


}
