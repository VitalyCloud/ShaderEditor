//
//  VertexArrayView.hpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 25.04.2022.
//

#ifndef VertexArrayView_hpp
#define VertexArrayView_hpp

#include "VertexView.hpp"
#include "IndexView.hpp"
#include "Core/Core.hpp"

namespace Editor {

class VertexArrayView {
public:
    VertexArrayView();
    ~VertexArrayView();
    
    void Draw();

private:
    VertexView m_VertexView;
    IndexView m_IndexView;
    
    std::vector<Core::Ref<VertexBuffer>> m_VertexBuffers;
    std::vector<uint32_t> m_Indicies;
};

}



#endif /* VertexArrayView_hpp */
