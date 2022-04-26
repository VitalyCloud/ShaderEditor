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
#include "UniformBufferConteiner.hpp"

#include "Renderer/OrthographicCamera.hpp"

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
    
    Core::Ref<UniformBufferConteiner> m_UniformBuffer;
    
    Core::Ref<Renderer::OrthographicCamera> m_Camera;
    glm::vec3 m_CameraPostion = glm::vec3(0.0);
    
};

}

#endif /* Pipeline_hpp */
