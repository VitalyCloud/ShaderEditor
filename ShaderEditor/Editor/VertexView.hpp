//
//  VertexView.hpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 01.05.2022.
//

#ifndef VertexView_hpp
#define VertexView_hpp

#include "InspectorPanel.hpp"
#include "VertexBufferContainer.hpp"
#include "Core/Memory.hpp"

namespace Editor {

class VertexView: public Inspector {
public:
    void Draw() override;

    void SetContext(const Core::Ref<VertexBufferContainer>& context) { m_VertexBufferContext = context; }
private:
    bool DrawTableDataInput(const VertexBufferElement& element, void* data);
    void DrawBufferElementPopup(int index);

private:
    Core::Ref<VertexBufferContainer> m_VertexBufferContext = nullptr;
    int m_VertexIndexToDelete = -1;
    int m_VertexLayoutElementToDelete = -1;
};

}

#endif /* VertexView_hpp */
