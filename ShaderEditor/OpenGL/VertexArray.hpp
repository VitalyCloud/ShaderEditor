//
//  VertexArray.hpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 21.04.2022.
//

#ifndef VertexArray_hpp
#define VertexArray_hpp

#include <vector>
#include "Core/Memory.hpp"
#include "Buffer.hpp"

namespace OpenGL {

class VertexArray {
public:
    VertexArray();
    ~VertexArray();
    
    void Bind();
    void Unbind();
    
    void AddVertexBuffer(const Engine::Ref<VertexBuffer>& vertexBuffer);
    void SetIndexBuffer(const Engine::Ref<IndexBuffer>& indexBuffer);
    
    const std::vector<Engine::Ref<VertexBuffer>>& GetVertexBuffers() const { return m_VertexBuffers; }
    const Engine::Ref<IndexBuffer>& GetIndexBuffer() const { return m_IndexBuffer; }
    
private:
    uint32_t m_RendererID;
    uint32_t m_VertexBufferIndex = 0;
    std::vector<Engine::Ref<VertexBuffer>> m_VertexBuffers;
    Engine::Ref<IndexBuffer> m_IndexBuffer;
};

}
#endif /* VertexArray_hpp */
