//
//  IndexView.hpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 25.04.2022.
//

#ifndef IndexView_hpp
#define IndexView_hpp

#include "Core/Core.hpp"

namespace Editor {

class IndexView {
public:
    IndexView();
    ~IndexView();
    
    void Draw();
    
    bool IsChanged() { return m_Changed; }
    void SetContext(const Core::Ref<std::vector<uint32_t>>& context) { m_Context = context; }
    
private:
    Core::Ref<std::vector<uint32_t>> m_Context = nullptr;
    
    bool m_AutoChange = false;
    bool m_Changed = false;
};

}

#endif /* IndexView_hpp */
