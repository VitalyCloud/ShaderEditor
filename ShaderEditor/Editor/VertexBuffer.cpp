//
//  VertexBuffer.cpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 24.04.2022.
//
#include "pch.h"
#include "VertexBuffer.hpp"

namespace Editor {

VertexBuffer::VertexBuffer(int reserve) {
    PushLayoutElement(OpenGL::BufferElement(OpenGL::ShaderDataType::Float, "Pos"));
    
    // Reserve buffer
    uint32_t vertexSize = GetVertexSize();
    m_Buffer.reserve(vertexSize * reserve);
    
    PushVertex();
}

VertexBuffer::~VertexBuffer() {}

void VertexBuffer::PushLayoutElement(const OpenGL::BufferElement& element) {
    m_Layout.push_back(VertexBufferElement(element));
    m_VertexSize = GetVertexSize();
    
    int elementIndex = (int)m_Layout.size() - 1;
    
    auto elementOffset = GetLayoutElementOffsetForIndex(elementIndex);
    auto elementSize = m_Layout[elementIndex].Element.Size;
    
    m_Buffer.reserve(m_VertexSize * m_VertexCount);
    auto iterator = m_Buffer.begin();
    for(int i=0; i<m_VertexCount; i++, iterator += m_VertexSize) {
        m_Buffer.insert(iterator + elementOffset, elementSize, 0);
    }
}

void VertexBuffer::InsertLayoutElement(int position, OpenGL::BufferElement& element) {
    m_Layout.insert(m_Layout.begin() + position, element);
    m_VertexSize = GetVertexSize();
    
    auto elementOffset = GetLayoutElementOffsetForIndex(position);
    auto elementSize = m_Layout[position].Element.Size;
    
    m_Buffer.reserve(m_VertexSize * m_VertexCount);
    auto iterator = m_Buffer.begin();
    for(int i=0; i<m_VertexCount; i++, iterator += m_VertexSize) {
        m_Buffer.insert(iterator + elementOffset, elementSize, 0);
    }
}

void VertexBuffer::RemoveLayoutElement(int position) {
    auto elementOffset = GetLayoutElementOffsetForIndex(position);
    auto elementSize = m_Layout[position].Element.Size;
    
    auto iterator = m_Buffer.begin();
    for(int i=0; i<m_VertexCount; i++, iterator += m_VertexSize - elementSize) {
        m_Buffer.erase(iterator + elementOffset, iterator + elementOffset + elementSize);
    }
    
    m_Layout.erase(m_Layout.begin() + position);
    m_VertexSize = GetVertexSize();
}

void VertexBuffer::PushVertex() {
    m_Buffer.resize(m_Buffer.size() + GetVertexSize(), 0);
    m_VertexCount += 1;
}

void VertexBuffer::RemoveVertex(int position) {
    auto vertexOffset = m_VertexSize * position;
    auto begin = m_Buffer.begin();
    m_Buffer.erase(begin + vertexOffset, begin + vertexOffset + m_VertexSize);
    m_VertexCount -= 1;
}

void VertexBuffer::ChangeElementType(int position, OpenGL::ShaderDataType type) {
    OpenGL::BufferElement oldElement = m_Layout[position].Element;
    OpenGL::BufferElement newElement = OpenGL::BufferElement(type, oldElement.Name, oldElement.Normalized);
    
    RemoveLayoutElement(position);
    InsertLayoutElement(position, newElement);
}

char* VertexBuffer::GetVertexComponent(int vertexIndex, int componentIndex) {
    return m_Buffer.data() + GetVertexOffsetForIndex(vertexIndex) + GetLayoutElementOffsetForIndex(componentIndex);
}

uint32_t VertexBuffer::GetVertexSize() {
    uint32_t size = 0;
    for(auto& element: m_Layout) {
        size += element.Element.Size;
    }
    return size;
}

uint32_t VertexBuffer::GetVertexOffsetForIndex(int index) {
    return GetVertexSize() * index;
}

uint32_t VertexBuffer::GetLayoutElementOffsetForIndex(int index) {
    uint32_t offset = 0;
    for(int i=0; i<index; i++) {
        offset += m_Layout[i].Element.Size;
    }
    return offset;
}

}
