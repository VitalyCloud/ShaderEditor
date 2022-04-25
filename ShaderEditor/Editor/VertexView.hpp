//
//  VertexView.hpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 24.04.2022.
//

#ifndef VertexView_hpp
#define VertexView_hpp

#include "OpenGL/Buffer.hpp"
#include "VertexBufferConteiner.hpp"
#include "Core/Core.hpp"

namespace Editor {

class VertexView {
public:
    VertexView();
    ~VertexView();
    
    void Draw();
    
    void SetContext(const Core::Ref<VertexBufferConteiner> context) { m_VertexBufferContext = context; }
    
private:
    bool DrawTableDataInput(const VertexBufferElement& element, void* data);
    void DrawBufferElementPopup(int index);

private:
    Core::Ref<VertexBufferConteiner> m_VertexBufferContext = nullptr;
    
    bool m_AutoChange = false;
};

}

#endif /* VertexView_hpp */
