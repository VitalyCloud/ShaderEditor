//
//  PipelinePanel.hpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 01.05.2022.
//

#ifndef PipelinePanel_hpp
#define PipelinePanel_hpp

#include "Pipeline.hpp"
#include "Core/Memory.hpp"

#include "ShaderPass.hpp"
#include "Mesh.hpp"
#include "ShaderRenderState.hpp"


namespace Editor {

class PipelinePanel {
public:
    PipelinePanel();
    ~PipelinePanel();
    
    void Draw(const char* title, bool* p_open);
    
    void SerContext(const Core::Ref<Pipeline>& context) { m_Context = context; }
    
private:
    Core::Ref<Pipeline> m_Context = nullptr;
    ShaderPassInspector m_ShaderPassInspector;
    
    MeshInspector m_MeshInspector;
    ShaderRenderStateInspector m_RenderStateInspector;
    SceneCameraInspector m_CameraInspector;
};

}


#endif /* PipelinePanel_hpp */
