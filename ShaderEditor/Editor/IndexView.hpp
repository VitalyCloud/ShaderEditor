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
};

}

#endif /* IndexView_hpp */
