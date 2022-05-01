//
//  IndexView.hpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 01.05.2022.
//

#ifndef IndexView_hpp
#define IndexView_hpp

#include "InspectorPanel.hpp"
#include "IndexBufferContainer.hpp"
#include "Core/Memory.hpp"

namespace Editor {

class IndexView: public Inspector {
public:
    
    void Draw() override;
    void SetContext(const Core::Ref<IndexBufferContainer>& context) { m_Context = context; }
    
private:
    Core::Ref<IndexBufferContainer> m_Context = nullptr;
};


}

#endif /* IndexView_hpp */
