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
    
    void AddVertexBuffer(const Core::Ref<VertexBuffer>& vertexBuffer);
    void SetIndexBuffer(const Core::Ref<IndexBuffer>& indexBuffer);
    
    const std::vector<Core::Ref<VertexBuffer>>& GetVertexBuffers() const { return m_VertexBuffers; }
    const Core::Ref<IndexBuffer>& GetIndexBuffer() const { return m_IndexBuffer; }
    
    static uint32_t GetActiveObjectsCount() { return s_ActviveObjectsCount; }
private:
    static uint32_t s_ActviveObjectsCount;
    
    uint32_t m_RendererID;
    uint32_t m_VertexBufferIndex = 0;
    std::vector<Core::Ref<VertexBuffer>> m_VertexBuffers;
    Core::Ref<IndexBuffer> m_IndexBuffer;
};

}
#endif /* VertexArray_hpp */
