//
//  Pipeline.hpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 01.05.2022.
//

#ifndef Pipeline_hpp
#define Pipeline_hpp

#include "ShaderPass.hpp"

#include <vector>

namespace Editor {

class Pipeline {
public:
    Pipeline();
    ~Pipeline();
    
    void OnUpdate();
    
    std::vector<Core::Ref<ShaderPass>>& GetShaderPasses() { return m_ShaderPasses; }
private:
    std::vector<Core::Ref<ShaderPass>> m_ShaderPasses;
    
};

}

#endif /* Pipeline_hpp */
