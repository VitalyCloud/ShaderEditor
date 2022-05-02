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
#include "InspectorPanel.hpp"


#include <vector>
namespace Editor {

enum class IndexBufferState {
    None = 0,
    DataChanged = BIT(0),
    SizeChanged = BIT(1)
};

class IndexBufferContainer;

class IndexView: public Inspector {
public:
    
    void Draw() override;
    void SetContext(const Core::Ref<IndexBufferContainer>& context) { m_Context = context; }
    
private:
    Core::Ref<IndexBufferContainer> m_Context = nullptr;
};



class IndexBufferContainer {
public:
    IndexBufferContainer();
    ~IndexBufferContainer();
  
    void SetData(const std::vector<uint32_t>& data);
    
    BufferState<IndexBufferState>& GetState() { return m_State; }
    const Core::Ref<OpenGL::IndexBuffer>& GetIB() { return m_IndexBuffer; }
    
    void UpdateIndexBufferIfNeeded();
    
private:
    friend IndexView;
    Core::Ref<OpenGL::IndexBuffer> m_IndexBuffer = nullptr;
    std::vector<uint32_t> m_Data;
    
    BufferState<IndexBufferState> m_State;
};



}




#endif /* IndexBufferContainer_hpp */
