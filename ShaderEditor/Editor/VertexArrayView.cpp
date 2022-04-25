//
//  VertexArrayView.cpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 26.04.2022.
//

#include "Core/pch.h"
#include "VertexArrayView.hpp"

#include "imgui.h"

namespace Editor {
VertexArrayView::VertexArrayView() {
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

    m_Indicies = {0, 1, 2};
    InvalidateVertexArray();
}

VertexArrayView::~VertexArrayView() {
    
}

void VertexArrayView::InvalidateVertexArray() {
    EN_INFO("Invalidating VBs Changes");
    m_OpenGLVA = Core::CreateRef<OpenGL::VertexArray>();
    for(auto buffer: m_VertexBuffers) {
        auto vb = buffer->CreateOpenGLVertexBuffer();
        m_OpenGLVA->AddVertexBuffer(vb);
    }
    auto indexBuffer = Core::CreateRef<OpenGL::IndexBuffer>(m_Indicies.data(), m_Indicies.size());
    m_OpenGLVA->SetIndexBuffer(indexBuffer);
}

void VertexArrayView::AddVertexBuffer() {
    EN_INFO("AddVertexBuffer");
    auto newBuffer = Core::CreateRef<VertexBufferConteiner>();
    newBuffer->SetChanged(false);
    m_VertexBuffers.push_back(newBuffer);
    
    InvalidateVertexArray();
}

void VertexArrayView::RemoveVertexBuffer(int index) {
    EN_INFO("RemoveVertexBuffer");
    m_VertexBuffers.erase(m_VertexBuffers.begin() + index);
    InvalidateVertexArray();
}

void VertexArrayView::Draw() {
    ImGui::Separator();
    for(int i=0; i<m_VertexBuffers.size(); i++) {
        auto title = fmt::format("Buffer #{0}", i);
        bool close = true;
        
        bool open = false;
        if(m_VertexBuffers.size() == 1)
            open = ImGui::CollapsingHeader(title.c_str());
        else
            open = ImGui::CollapsingHeader(title.c_str(), &close);
        
        if(ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
            ImGui::OpenPopup("BufferContext");
        }
        
        if(open) {
            ImGui::PushID(i);
            m_VertexView.SetContext(m_VertexBuffers[i]);
            m_VertexView.Draw();
            ImGui::PopID();
        }
        
        if(!close && m_VertexBuffers.size() > 1) {
            RemoveVertexBuffer(i);
        }
        if(m_VertexBuffers[i]->IsChanged()) {
            InvalidateVertexArray();
            m_VertexBuffers[i]->SetChanged(false);
        }
            
    }
   
    if(ImGui::BeginPopup("BufferContext")) {
        if(ImGui::Selectable("Add Buffer")) {
            AddVertexBuffer();
        }
        ImGui::EndPopup();
    }
    
    if(ImGui::CollapsingHeader("Index Buffer")) {
        m_IndexView.Draw(m_Indicies);
        if(m_IndexView.IsChanged()) {
            InvalidateVertexArray();
        }
    }
}
}
