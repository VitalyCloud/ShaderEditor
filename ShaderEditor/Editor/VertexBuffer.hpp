//
//  VertexBuffer.hpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 24.04.2022.
//

#ifndef VertexBuffer_hpp
#define VertexBuffer_hpp

#include "OpenGL/Buffer.hpp"


namespace Editor {

class VertexBuffer
{
public:
    VertexBuffer(int reserve = 10) {
        PushLayoutElement(OpenGL::BufferElement(OpenGL::ShaderDataType::Float, "Pos"));
        
        // Reserve buffer
        uint32_t vertexSize = GetVertexSize();
        m_Buffer.reserve(vertexSize * reserve);
        
        PushVertex();
    }
    
    ~VertexBuffer() {}
    
    void PushLayoutElement(const OpenGL::BufferElement& element) {
        m_Layout.push_back(element);
        m_VertexSize = GetVertexSize();
        
        int elementIndex = (int)m_Layout.size() - 1;
        
        auto elementOffset = GetLayoutElementOffsetForIndex(elementIndex);
        auto elementSize = m_Layout[elementIndex].Size;
        
        m_Buffer.reserve(m_VertexSize * m_VertexCount);
        auto iterator = m_Buffer.begin();
        for(int i=0; i<m_VertexCount; i++, iterator += m_VertexSize) {
            m_Buffer.insert(iterator + elementOffset, elementSize, 0);
        }
    }
    
    void InsertLayoutElement(int position, OpenGL::BufferElement& element) {
        m_Layout.insert(m_Layout.begin() + position, element);
        m_VertexSize = GetVertexSize();
        
        auto elementOffset = GetLayoutElementOffsetForIndex(position);
        auto elementSize = m_Layout[position].Size;
        
        m_Buffer.reserve(m_VertexSize * m_VertexCount);
        auto iterator = m_Buffer.begin();
        for(int i=0; i<m_VertexCount; i++, iterator += m_VertexSize) {
            m_Buffer.insert(iterator + elementOffset, elementSize, 0);
        }
    }
    
    void RemoveLayoutElement(int position) {
        auto elementOffset = GetLayoutElementOffsetForIndex(position);
        auto elementSize = m_Layout[position].Size;
        
        auto iterator = m_Buffer.begin();
        for(int i=0; i<m_VertexCount; i++, iterator += m_VertexSize - elementSize) {
            m_Buffer.erase(iterator + elementOffset, iterator + elementOffset + elementSize);
        }
        
        m_Layout.erase(m_Layout.begin() + position);
        m_VertexSize = GetVertexSize();
    }

    void PushVertex() {
        m_Buffer.resize(m_Buffer.size() + GetVertexSize(), 0);
        m_VertexCount += 1;
    }
    
    void RemoveVertex(int position) {
        auto vertexOffset = m_VertexSize * position;
        auto begin = m_Buffer.begin();
        m_Buffer.erase(begin + vertexOffset, begin + vertexOffset + m_VertexSize);
        m_VertexCount -= 1;
    }
    
    void ChangeElementType(int position, OpenGL::ShaderDataType type) {
        OpenGL::BufferElement oldElement = m_Layout[position];
        OpenGL::BufferElement newElement = OpenGL::BufferElement(type, oldElement.Name, oldElement.Normalized);
        
        RemoveLayoutElement(position);
        InsertLayoutElement(position, newElement);
    }
    
    char* GetVertexComponent(int vertexIndex, int componentIndex) {
        return m_Buffer.data() + GetVertexOffsetForIndex(vertexIndex) + GetLayoutElementOffsetForIndex(componentIndex);
    }
    
    char* GetVertex(int index) { return m_Buffer.data() + GetVertexOffsetForIndex(index); }
    uint32_t VertexCount() { return m_VertexCount; }
    
    std::vector<OpenGL::BufferElement>& GetLayoutElements() { return m_Layout; }
    
private:
    uint32_t GetVertexSize() {
        uint32_t size = 0;
        for(auto& element: m_Layout) {
            size += element.Size;
        }
        return size;
    }
    
    uint32_t GetVertexOffsetForIndex(int index) {
        return GetVertexSize() * index;
    }
    
    uint32_t GetLayoutElementOffsetForIndex(int index) {
        uint32_t offset = 0;
        for(int i=0; i<index; i++) {
            offset += m_Layout[i].Size;
        }
        return offset;
    }
    
private:
    std::vector<OpenGL::BufferElement> m_Layout;
    std::vector<char> m_Buffer;
    
    uint32_t m_VertexCount = 0;
    uint32_t m_VertexSize = 0;
};

}

#endif /* VertexBuffer_hpp */
