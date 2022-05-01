//
//  IndexBufferContainer.hpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 01.05.2022.
//

#ifndef IndexBufferContainer_hpp
#define IndexBufferContainer_hpp

#include "BufferState.hpp"
#include "OpenGL/Buffer.hpp"

#include <vector>

enum class IndexBufferState {
    None = 0,
    DataChanged = BIT(0),
    SizeChanged = BIT(1)
};

namespace Editor {

class IndexBufferContainer {
public:
    IndexBufferContainer();
    ~IndexBufferContainer();
  
    std::vector<uint32_t>& GetData() { return m_Data; }
    BufferState<IndexBufferState>& GetState() { return m_State; }
    const Core::Ref<OpenGL::IndexBuffer>& GetIB() { return m_IndexBuffer; }
    
    void UpdateIndexBufferIfNeeded();
    
private:
    Core::Ref<OpenGL::IndexBuffer> m_IndexBuffer = nullptr;
    std::vector<uint32_t> m_Data;
    
    BufferState<IndexBufferState> m_State;
};

}




#endif /* IndexBufferContainer_hpp */
