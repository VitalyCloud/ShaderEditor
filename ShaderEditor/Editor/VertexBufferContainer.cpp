//
//  VertexBufferContainer.cpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 01.05.2022.
//

#include "VertexBufferContainer.hpp"

namespace Editor {

VertexBufferContainer::VertexBufferContainer() {
    m_VertexBuffer = Core::CreateRef<OpenGL::VertexBuffer>(0);
}

VertexBufferContainer::~VertexBufferContainer() {
    
}

void VertexBufferContainer::PushLayoutElement(const OpenGL::BufferElement& element) {
    
    m_Layout.emplace_back(element);
    m_VertexSize = CalculateVertexSize();
    
    int elementIndex = (int)m_Layout.size() - 1;
    
    auto elementOffset = GetLayoutElementOffsetForIndex(elementIndex);
    auto elementSize = m_Layout[elementIndex].Element.Size;
    
    m_Data.reserve(m_VertexSize * m_VertexCount);
    auto iterator = m_Data.begin();
    for(int i=0; i<m_VertexCount; i++, iterator += m_VertexSize) {
        m_Data.insert(iterator + elementOffset, elementSize, 0);
    }
}



void VertexBufferContainer::PopLayoutElement(int index) {
    EN_ASSERT(index < m_Layout.size(), "Component index is out of range");
    auto elementOffset = GetLayoutElementOffsetForIndex(index);
    auto elementSize = m_Layout[index].Element.Size;
    
    auto iterator = m_Data.begin();
    for(int i=0; i<m_VertexCount; i++, iterator += m_VertexSize - elementSize) {
        m_Data.erase(iterator + elementOffset, iterator + elementOffset + elementSize);
    }
    
    m_Layout.erase(m_Layout.begin() + index);
    m_VertexSize = CalculateVertexSize();
}

void VertexBufferContainer::ChangeLayoutElementType(int index, OpenGL::ShaderDataType newType) {
    EN_ASSERT(index < m_Layout.size(), "Component index is out of range");;
    OpenGL::BufferElement oldElement = m_Layout[index].Element;
    OpenGL::BufferElement newElement = OpenGL::BufferElement(newType, oldElement.Name, oldElement.Normalized);
    
    PopLayoutElement(index);
    InsertLayoutElement(index, newElement);
}

void VertexBufferContainer::InsertLayoutElement(int position, const OpenGL::BufferElement& element) {
    EN_ASSERT(position <= m_Layout.size(), "Component index is out of range");
    m_Layout.insert(m_Layout.begin() + position, element);
    m_VertexSize = CalculateVertexSize();
    
    auto elementOffset = GetLayoutElementOffsetForIndex(position);
    auto elementSize = m_Layout[position].Element.Size;
    
    m_Data.reserve(m_VertexSize * m_VertexCount);
    auto iterator = m_Data.begin();
    for(int i=0; i<m_VertexCount; i++, iterator += m_VertexSize) {
        m_Data.insert(iterator + elementOffset, elementSize, 0);
    }
}

void VertexBufferContainer::PushVertex(VertexData* value, int size) {
    m_Data.resize(m_Data.size() + m_VertexSize, 0);
    m_VertexCount += 1;
    if(value != nullptr) {
        SetVertex(m_VertexCount-1, value, size);
    }
}

void VertexBufferContainer::SetVertex(int index, VertexData* value, int size) {
    EN_ASSERT(value != nullptr, "Vertex data is null");
    EN_ASSERT(size == m_VertexSize, "Vertex sizes don't match");
    EN_ASSERT(index < m_VertexCount, "Index is out of range");
    
    uint32_t begin = GetVertexOffsetForIndex(index);
    for(int i=0; i<size; i++) {
        m_Data[begin + i] = *(value + i);
    }
}

void VertexBufferContainer::PopVertex(int index) {
    if(index < 0)
        index = m_VertexSize - 1;
    if(index > -1) {
        EN_ASSERT(index < m_VertexCount, "Index is out of range");
        auto vertexOffset = m_VertexSize * index;
        auto begin = m_Data.begin();
        m_Data.erase(begin + vertexOffset, begin + vertexOffset + m_VertexSize);
        m_VertexCount -= 1;
    }
}

VertexBufferContainer::VertexData* VertexBufferContainer::GetVertexData(int index) {
    EN_ASSERT(index < m_VertexCount, "Index is out of range");
    return m_Data.data() + GetVertexOffsetForIndex(index);
}

VertexBufferContainer::VertexData* VertexBufferContainer::GetVertexComponent(int index, int componentIndex) {
    EN_ASSERT(index < m_VertexCount, "Index is out of range");
    EN_ASSERT(componentIndex < m_Layout.size(), "Component index is out of range");
    return m_Data.data() + GetVertexOffsetForIndex(index) + GetLayoutElementOffsetForIndex(componentIndex);
}

uint32_t VertexBufferContainer::CalculateVertexSize() {
    uint32_t size = 0;
    for(auto& element: m_Layout) {
        size += element.Element.Size;
    }
    return size;
}

uint32_t VertexBufferContainer::GetVertexOffsetForIndex(int index) {
    EN_ASSERT(index < m_VertexCount, "Index is out of range");
    return m_VertexSize * index;
}

uint32_t VertexBufferContainer::GetLayoutElementOffsetForIndex(int index) {
    EN_ASSERT(index < m_Layout.size(), "Component index is out of range");
    uint32_t offset = 0;
    for(int i=0; i<index; i++) {
        offset += m_Layout[i].Element.Size;
    }
    return offset;
}

void VertexBufferContainer::UpdateVertexBuffer() {
    m_VertexBuffer->Resize(m_VertexSize * m_VertexCount);
    m_VertexBuffer->SetData(m_Data.data(), m_VertexSize * m_VertexCount);
    
    std::vector<OpenGL::BufferElement> layout;
    layout.reserve(m_Layout.size());
    for(auto& i : m_Layout)
        layout.push_back(i.Element);
    
    m_VertexBuffer->SetLayout(layout);
}

void VertexBufferContainer::UpdateVertexBufferIfNeeded() {
    if(m_Data.size() == 0 || m_Layout.size() == 0)
        return;
    
    if(m_State.CheckIf(VertexBufferState::LayoutChanged)) {
        EN_INFO("Handle VBC layout change");
        m_VertexBuffer->Resize(m_VertexSize * m_VertexCount);
        m_VertexBuffer->SetData(m_Data.data(), m_VertexSize * m_VertexCount);
        
        std::vector<OpenGL::BufferElement> layout;
        layout.reserve(m_Layout.size());
        for(auto& i : m_Layout)
            layout.push_back(i.Element);
        
        m_VertexBuffer->SetLayout(layout);
        return;
    }
    
    if(m_State.CheckIf(VertexBufferState::SizeChanged)) {
        EN_INFO("Handle VBC size change");
        m_VertexBuffer->Resize(m_VertexSize * m_VertexCount);
        m_VertexBuffer->SetData(m_Data.data(), m_VertexSize * m_VertexCount);
        return;
    }
    
    if(m_State.CheckIf(VertexBufferState::DataChanged)) {
        EN_INFO("Handle VBC data change");
        m_VertexBuffer->SetData(m_Data.data(), m_VertexSize * m_VertexCount);
        return;
    }
}
}
