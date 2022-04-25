//
//  VertexBuffer.hpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 24.04.2022.
//

#ifndef VertexBuffer_hpp
#define VertexBuffer_hpp

#include "OpenGL/Buffer.hpp"
#include "ImGuiInputSettings.hpp"

namespace Editor {

struct VertexBufferElement {
    InputSettings Settings;
    OpenGL::BufferElement Element;
    VertexBufferElement(const OpenGL::BufferElement& element) : Element(element) {}
};

class VertexBufferConteiner
{
public:
    VertexBufferConteiner(int reserve = 10);
    ~VertexBufferConteiner();
    void PushLayoutElement(const OpenGL::BufferElement& element);
    void InsertLayoutElement(int position, OpenGL::BufferElement& element);
    void RemoveLayoutElement(int position);
    void ChangeElementType(int position, OpenGL::ShaderDataType type);
    
    void PushVertex();
    void RemoveVertex(int position);
    char* GetVertex(int index) { return m_Buffer.data() + GetVertexOffsetForIndex(index); }
    char* GetVertexComponent(int vertexIndex, int componentIndex);

    uint32_t VertexCount() { return m_VertexCount; }
    
    std::vector<VertexBufferElement>& GetLayoutElements() { return m_Layout; }
    
    void SetChanged(bool status) { m_Changed = status; }
    bool IsChanged() { return m_Changed; }
    
    Core::Ref<OpenGL::VertexBuffer> CreateOpenGLVertexBuffer();
private:
    uint32_t GetVertexSize();
    uint32_t GetVertexOffsetForIndex(int index);
    uint32_t GetLayoutElementOffsetForIndex(int index);
    
private:
    std::vector<VertexBufferElement> m_Layout;
    std::vector<char> m_Buffer;
    
    uint32_t m_VertexCount = 0;
    uint32_t m_VertexSize = 0;
    
    bool m_Changed = false;
};

}

#endif /* VertexBuffer_hpp */
