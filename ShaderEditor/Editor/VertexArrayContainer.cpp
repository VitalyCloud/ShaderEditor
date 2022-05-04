//
//  VertexArrayContainer.cpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 01.05.2022.
//
#include "pch.h"
#include "VertexArrayContainer.hpp"
#include "Core/Assert.h"


namespace Editor {

VertexArrayContainer::VertexArrayContainer() {
//    m_VertexArray = Core::CreateRef<OpenGL::VertexArray>();
    SetIndexBuffer(Core::CreateRef<IndexBufferContainer>());
}

VertexArrayContainer::~VertexArrayContainer() {
    
}

void VertexArrayContainer::InvalidateVertexArray() {
    m_VertexArray = nullptr;
    m_VertexArray = Core::CreateRef<OpenGL::VertexArray>();
    for(auto buffer: m_VertexContainers) {
        if(buffer->GetLayoutElements().size() > 0) {
            m_VertexArray->AddVertexBuffer(buffer->GetVB());
        }
    }
    
    // Index buffer
    if(m_IndexContainer != nullptr && m_IndexContainer->GetIB() != nullptr)
        m_VertexArray->SetIndexBuffer(m_IndexContainer->GetIB());
}

void VertexArrayContainer::AddVertexBuffer(const Core::Ref<VertexBufferContainer>& vb) {
    
    Core::Ref<VertexBufferContainer> vertexBuffer;
    if(vb != nullptr)
        vertexBuffer = vb;
    else
        vertexBuffer = Core::CreateRef<VertexBufferContainer>();
    
    m_VertexContainers.push_back(vertexBuffer);
    InvalidateVertexArray();
}

void VertexArrayContainer::SetIndexBuffer(const Core::Ref<IndexBufferContainer>& ib) {
    
    m_IndexContainer = ib;
    
    if(m_VertexArray!=nullptr)
        m_VertexArray->SetIndexBuffer(m_IndexContainer->GetIB());
}

void VertexArrayContainer::RemoveVertexBuffer(int index) {
//    EN_ASSERT(m_VertexContainers.size() < index, "");
    m_VertexContainers.erase(m_VertexContainers.begin() + index);
    InvalidateVertexArray();
}


}
