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

#include "Renderer/OrthographicCamera.hpp"
#include "InspectorPanel.hpp"

namespace Editor {

class CameraInspector: public InspectorItem {
public:
    void Draw() override;
    
    void SetContext(const Core::Ref<Renderer::OrthographicCamera>& context) { m_Context = context; }
private:
    Core::Ref<Renderer::OrthographicCamera> m_Context = nullptr;
    glm::vec3 m_CameraPostion = glm::vec3(0.0);
    float rotation = 0.0f;
};


class PipelinePanel {
public:
    PipelinePanel();
    ~PipelinePanel();
    
    void OnUpdate();
    void Draw(const char* title, bool* p_open = nullptr);
private:
    // Camera
    Core::Ref<Renderer::OrthographicCamera> m_Camera;
    CameraInspector m_CameraInpector;
    
    // ShaderPass
    std::vector<Core::Ref<ShaderPass>> m_ShaderPasses;
    ShaderPassInspector m_ShaderPassInspector;
    MeshInspector m_MeshInspector;
};

}

#endif /* Pipeline_hpp */
