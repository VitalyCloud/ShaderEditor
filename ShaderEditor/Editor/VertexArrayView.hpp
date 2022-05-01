//
//  VertexArrayView.hpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 01.05.2022.
//

#ifndef VertexArrayView_hpp
#define VertexArrayView_hpp

#include "VertexArrayContainer.hpp"
#include "InspectorPanel.hpp"

#include "VertexView.hpp"
#include "IndexView.hpp"

namespace Editor {

class VertexArrayView: public Inspector {
public:
    void Draw() override;
    
    void SetContext(const Core::Ref<VertexArrayContainer>& context) { m_Context = context; }
    
private:
    Core::Ref<VertexArrayContainer> m_Context = nullptr;
    int m_VertexContainerToDelete = -1;
    
    VertexView m_VertexView;
    IndexView m_IndexView;
    
    bool m_AutoChange = false;
    
};


}

#endif /* VertexArrayView_hpp */
