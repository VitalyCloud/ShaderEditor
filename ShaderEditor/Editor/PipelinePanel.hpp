//
//  Pipeline.hpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 25.04.2022.
//

#ifndef Pipeline_hpp
#define Pipeline_hpp

#include <stdio.h>

#include "ShaderPass.hpp"

namespace Editor {

class PipelinePanel {
public:
    PipelinePanel();
    ~PipelinePanel();
    
    void OnUpdate();
    void Draw(const char* title, bool* p_open = nullptr);
    void DrawInspector(const char* title, bool* p_open = nullptr);
    
private:
    std::vector<Core::Ref<ShaderPass>> m_ShaderPassses;
    Core::Ref<ShaderPass> m_SelectedShaderPasss;
};

}

#endif /* Pipeline_hpp */
