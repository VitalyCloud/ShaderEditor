//
//  VertexArrayView.hpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 26.04.2022.
//

#ifndef VertexArrayView_hpp
#define VertexArrayView_hpp

#include "Core/Core.hpp"
#include "OpenGL/OpenGL.hpp"

#include "VertexView.hpp"
#include "IndexView.hpp"

namespace Editor {

class VertexArrayView {
public:
    VertexArrayView();
    ~VertexArrayView();
    
    void InvalidateVertexArray();
    
    void Draw();
    
    const Core::Ref<OpenGL::VertexArray>& GetOpenGLVA() { return m_OpenGLVA; }
    uint32_t GetVertexCount() { return m_VertexBuffers[0]->VertexCount(); }
    
private:
    void AddVertexBuffer();
    void RemoveVertexBuffer(int index);
    void InvalidateChanges(int index);

private:
    VertexView m_VertexView;
    std::vector<Core::Ref<VertexBufferConteiner>> m_VertexBuffers;
    
    IndexView m_IndexView;
    Core::Ref<std::vector<uint32_t>> m_Indicies;
    
    Core::Ref<OpenGL::VertexArray> m_OpenGLVA;
};

}

#endif /* VertexArrayView_hpp */
