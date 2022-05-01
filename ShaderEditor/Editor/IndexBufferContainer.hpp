//
//  IndexBufferContainer.hpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 01.05.2022.
//

#ifndef IndexBufferContainer_hpp
#define IndexBufferContainer_hpp

#include <vector>

namespace Editor {

class IndexBufferContainer {
public:
    IndexBufferContainer();
    ~IndexBufferContainer();
  
    std::vector<uint32_t>& GetData() { return m_Data; }
private:
    std::vector<uint32_t> m_Data;
};

}




#endif /* IndexBufferContainer_hpp */
