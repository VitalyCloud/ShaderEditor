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

namespace Editor {

class VertexView {
public:
    VertexView();
    ~VertexView();
    
    void Draw();
    
private:
    void DrawTableDataInput(const VertexBufferElement& element, void* data);
    void DrawBufferElementPopup(int index);

private:
    VertexBuffer m_VertexBuffer;
};

}

#endif /* VertexView_hpp */
