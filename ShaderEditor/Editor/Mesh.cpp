//
//  Mesh.cpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 27.04.2022.
//

#include "Core/pch.h"
#include "Mesh.hpp"

#include "ImGuiHelper.h"

namespace Editor {

Mesh::Mesh(const std::string& title) : m_Title(title) {
    m_Indicies = Core::CreateRef<std::vector<uint32_t>>();
    AddVertexBuffer();
    
    auto vb = m_VertexBuffers[0];
    
    vb->ChangeElementType(0, OpenGL::ShaderDataType::Float2);
    vb->PushLayoutElement({OpenGL::ShaderDataType::Float3, "Color"});
    vb->PushVertex();
    vb->PushVertex();

    float* pos0 = (float*)vb->GetVertexComponent(0, 0);
    *(pos0+0) = -0.5;
    *(pos0+1) = 0.5;

    float* pos1 = (float*)vb->GetVertexComponent(1, 0);
    *(pos1+0) = 0.5;
    *(pos1+1) = 0.5;

    float* pos2 = (float*)vb->GetVertexComponent(2, 0);
    *(pos2+0) = 0.0;
    *(pos2+1) = -0.5;
    
    float* color0 = (float*)vb->GetVertexComponent(0, 1);
    *(color0+0) = 1.0;
    *(color0+1) = 0.0;
    *(color0+2) = 0.0;

    float* color1 = (float*)vb->GetVertexComponent(1, 1);
    *(color1+0) = 0.0;
    *(color1+1) = 1.0;
    *(color1+2) = 0.0;

    float* color2 = (float*)vb->GetVertexComponent(2, 1);
    *(color2+0) = 0.0;
    *(color2+1) = 0.0;
    *(color2+2) = 1.0;

    *m_Indicies = {0, 1, 2};
    InvalidateVertexArray();
}

Mesh::~Mesh() {
    
}


void Mesh::InvalidateVertexArray() {
    EN_INFO("Invalidating Vertex Array Changes");
    m_VertexArray = Core::CreateRef<OpenGL::VertexArray>();
    for(auto buffer: m_VertexBuffers) {
        auto vb = buffer->CreateOpenGLVertexBuffer();
        m_VertexArray->AddVertexBuffer(vb);
    }
    auto indexBuffer = Core::CreateRef<OpenGL::IndexBuffer>(m_Indicies->data(), m_Indicies->size());
    m_VertexArray->SetIndexBuffer(indexBuffer);
}

void Mesh::AddVertexBuffer() {
    EN_INFO("Add new vertex buffer");
    auto newBuffer = Core::CreateRef<VertexBufferConteiner>();
    newBuffer->SetChanged(false);
    m_VertexBuffers.push_back(newBuffer);
    
    InvalidateVertexArray();
}

void Mesh::RemoveVertexBuffer(int index) {
    EN_INFO("Remove vertex buffer at index {0}", index);
    m_VertexBuffers.erase(m_VertexBuffers.begin() + index);
    InvalidateVertexArray();
}

void MeshInspector::Draw() {
    if(m_Context == nullptr)
        return;
    
    ImGui::InputText("Title##MeshTitle", &m_Context->m_Title);
    
    for(int i=0; i<m_Context->m_VertexBuffers.size(); i++) {
        auto title = fmt::format("Buffer #{0}", i);
        bool close = true;
        
        bool open = false;
        if(m_Context->m_VertexBuffers.size() == 1)
            open = ImGui::CollapsingHeader(title.c_str());
        else
            open = ImGui::CollapsingHeader(title.c_str(), &close);
        
        if(ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
            ImGui::OpenPopup("BufferContext");
        }
        
        if(open) {
            ImGui::PushID(i);
            m_VertexView.SetContext(m_Context->m_VertexBuffers[i]);
            m_VertexView.Draw();
            ImGui::PopID();
        }
        
        if(!close && m_Context->m_VertexBuffers.size() > 1) {
            m_Context->RemoveVertexBuffer(i);
        }
        if(m_Context->m_VertexBuffers[i]->IsChanged()) {
            m_Context->InvalidateVertexArray();
            m_Context->m_VertexBuffers[i]->SetChanged(false);
        }
            
    }
   
    if(ImGui::BeginPopup("BufferContext")) {
        if(ImGui::Selectable("Add Buffer")) {
            m_Context->AddVertexBuffer();
        }
        ImGui::EndPopup();
    }
    
    if(ImGui::CollapsingHeader("Index Buffer")) {
        m_IndexView.SetContext(m_Context->m_Indicies);
        m_IndexView.Draw();
        if(m_IndexView.IsChanged()) {
            m_Context->InvalidateVertexArray();
        }
    }
}

}
