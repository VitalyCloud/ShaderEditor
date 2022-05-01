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
    m_VertexArray = Core::CreateRef<OpenGL::VertexArray>();
    AddVertexBuffer();
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
}

void VertexArrayContainer::AddVertexBuffer() {
    Core::Ref<VertexBufferContainer> vertexBuffer = Core::CreateRef<VertexBufferContainer>();
    m_VertexContainers.push_back(vertexBuffer);
    InvalidateVertexArray();
}

void VertexArrayContainer::RemoveVertexBuffer(int index) {
//    EN_ASSERT(m_VertexContainers.size() < index, "");
    m_VertexContainers.erase(m_VertexContainers.begin() + index);
    InvalidateVertexArray();
}


}
