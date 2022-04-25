//
//  VertexView.hpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 24.04.2022.
//

#ifndef VertexView_hpp
#define VertexView_hpp

#include "OpenGL/Buffer.hpp"
#include "VertexBuffer.hpp"
#include "Core/Core.hpp"

namespace Editor {

class VertexView {
public:
    VertexView();
    ~VertexView();
    
    void Draw();
    
    void SetContext(const Core::Ref<VertexBuffer> context) { m_VertexBufferContext = context; }
    
private:
    void DrawTableDataInput(const VertexBufferElement& element, void* data);
    void DrawBufferElementPopup(int index);

private:
    Core::Ref<VertexBuffer> m_VertexBufferContext = nullptr;
};

}

#endif /* VertexView_hpp */
