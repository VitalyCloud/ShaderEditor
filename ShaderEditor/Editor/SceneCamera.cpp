//
//  SceneCamera.cpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 08.05.2022.
//

#include "SceneCamera.hpp"
#include "PropertyTable.hpp"


namespace Editor {

const char* SCENE_CAMERA_TYPES[2] {
    "Orthographic", "Perspective"
};

SceneCamera::SceneCamera(CameraType type) : m_Type(type) {
    
}

SceneCamera::~SceneCamera() {
    
}

void SceneCamera::OnUpdate() {
    // Update camera
    switch(m_Type) {
        case Otrhographic: {
            m_OrthoCam.SetPostion(m_Transform.Translation);
            break;
        }
            
        case Perspective: {
            EN_ASSERT(false, "Perspective is not implemented");
            break;
        }
            
        default: {
            EN_ASSERT(false, "Unknown camera type");
            break;
        }
    }
}

void SceneCamera::Upload(const Core::Ref<OpenGL::Shader>& shader) {
    shader->Bind();
    switch(m_Type) {
        case Otrhographic: {
            shader->SetMat4("u_ViewProjection", m_OrthoCam.GetViewProjectionMatrix());
            break;
        }
            
        case Perspective: {
            EN_ASSERT(false, "Perspective is not implemented");
            break;
        }
            
        default: {
            EN_ASSERT(false, "Unknown camera type");
            break;
        }
    }
    shader->Unbind();
}

    
SceneCameraInspector::SceneCameraInspector() {
    ImGuiInputSettings settings;
    settings.Type = ImGuiWidgetType::Drag;
    settings.Speed = 0.050f;
    
    m_PositionSettings = settings;
    m_RrotationSettings = settings;
    m_ScaleSettings = settings;
}
    
void SceneCameraInspector::Draw() {
    if(m_Context == nullptr) return;
    
    ImGui::Text("Scene Camera");
    ImGui::Separator();
    
    if(PropertyTable::Begin("SceneCameraProperties")) {
        
        PropertyTable::Combo("Type", SCENE_CAMERA_TYPES, IM_ARRAYSIZE(SCENE_CAMERA_TYPES), m_Context->m_Type);
        
        bool clicked = false;
        PropertyTable::InputFloat("Position", &m_Context->m_Transform.Translation.x, &m_PositionSettings, 3, &clicked);
        if(clicked)
            ImGui::OpenPopup("CameraInputSettings");
    
        if(ImGui::BeginPopup("CameraInputSettings")) {
            ImGui::DrawInputSettingsView(OpenGL::ShaderDataType::Float3, &m_PositionSettings);
            ImGui::EndPopup();
        }
        
        
        PropertyTable::End();
    }
}

}
