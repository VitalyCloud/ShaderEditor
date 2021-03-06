//
//  VertexArrayContainer.hpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 01.05.2022.
//

#ifndef VertexArrayContainer_hpp
#define VertexArrayContainer_hpp

#include "VertexBufferContainer.hpp"
#include "IndexBufferContainer.hpp"
#include "OpenGL/VertexArray.hpp"
#include "Core/Memory.hpp"

#include <vector>

namespace Editor {

class VertexArrayContainer {
public:
    VertexArrayContainer();
    ~VertexArrayContainer();
    
    void AddVertexBuffer(const Core::Ref<VertexBufferContainer>& vb = nullptr);
    void SetIndexBuffer(const Core::Ref<IndexBufferContainer>& ib);
    void RemoveVertexBuffer(int index);
    void InvalidateVertexArray();
    
    const Core::Ref<IndexBufferContainer>& GetIndexConteiner() { return m_IndexContainer; }
    
    std::vector<Core::Ref<VertexBufferContainer>>& GetVertexConteiners() { return m_VertexContainers; }
    
    const Core::Ref<OpenGL::VertexArray>& GetVA() { return m_VertexArray; }
    
private:
    Core::Ref<OpenGL::VertexArray> m_VertexArray = nullptr;
    
    std::vector<Core::Ref<VertexBufferContainer>> m_VertexContainers;
    Core::Ref<IndexBufferContainer> m_IndexContainer;
};

}

#endif /* VertexArrayContainer_hpp */
