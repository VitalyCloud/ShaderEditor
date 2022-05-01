//
//  VertexBufferContainer.hpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 01.05.2022.
//

#ifndef VertexBufferContainer_hpp
#define VertexBufferContainer_hpp

#include "ImGuiHelper.hpp"
#include "OpenGL/Buffer.hpp"
#include "BufferState.hpp"

#include <vector>

namespace Editor {

enum class VertexBufferState {
    None = 0,
    LayoutChanged = BIT(0),
    SizeChanged = BIT(1),
    DataChanged = BIT(2)
};

struct VertexBufferElement {
    ImGuiInputSettings Settings;
    OpenGL::BufferElement Element;
  
    VertexBufferElement(const OpenGL::BufferElement& element)
    : Element(element) {}
};

class VertexBufferContainer  {
public:
    using VertexData = char;
    VertexBufferContainer();
    ~VertexBufferContainer();
    
    void PushLayoutElement(const OpenGL::BufferElement& element);
    void PopLayoutElement(int index);
    void ChangeLayoutElementType(int index, OpenGL::ShaderDataType newType);
    void InsertLayoutElement(int position, const OpenGL::BufferElement& element);
    
    void PushVertex(VertexData* value = nullptr, int size = 0);
    void SetVertex(int index, VertexData* value, int size);
    void PopVertex(int index = -1);
    VertexData* GetVertexData(int index);
    VertexData* GetVertexComponent(int index, int componentIndex);
    
    std::vector<VertexBufferElement>& GetLayoutElements() { return m_Layout; }
    uint32_t Count() { return m_VertexCount; }
    
    VertexData* operator[](int index) { return GetVertexData(index); }
    
    // Experimental
    void UpdateVertexBuffer();
    void UpdateVertexBufferIfNeeded();
    const Core::Ref<OpenGL::VertexBuffer>& GetVB() { return m_VertexBuffer; }
    const BufferState<VertexBufferState>& GetState() const { return m_State; }
    BufferState<VertexBufferState>& GetState() { return m_State; }
    bool IsLayoutChanged() { return m_State.CheckIf(VertexBufferState::LayoutChanged); }
    
private:
    uint32_t CalculateVertexSize();
    uint32_t GetVertexOffsetForIndex(int index);
    uint32_t GetLayoutElementOffsetForIndex(int index);
    
private:
    std::vector<VertexBufferElement> m_Layout;
    std::vector<VertexData> m_Data;
    
    uint32_t m_VertexCount = 0;
    uint32_t m_VertexSize = 0;
    
    BufferState<VertexBufferState> m_State;
    
    Core::Ref<OpenGL::VertexBuffer> m_VertexBuffer = nullptr;
};

}

#endif /* VertexBufferContainer_hpp */
