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
    
    void Draw(std::vector<uint32_t>& context);
    
    bool IsChanged() { return m_Changed; }
private:
    bool m_AutoChange = false;
    bool m_Changed = false;
};

}

#endif /* IndexView_hpp */
