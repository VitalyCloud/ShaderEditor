//
//  IndexView.hpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 25.04.2022.
//

#ifndef IndexView_hpp
#define IndexView_hpp

namespace Editor {

class IndexView {
public:
    IndexView();
    ~IndexView();
    
    void Draw();

private:
    std::vector<uint32_t> m_Indicies;
};

}

#endif /* IndexView_hpp */
