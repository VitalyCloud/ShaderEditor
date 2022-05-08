//
//  SceneCamera.hpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 08.05.2022.
//

#ifndef SceneCamera_hpp
#define SceneCamera_hpp

#include "InspectorPanel.hpp"
#include "Components.hpp"
#include "ImGuiHelper.hpp"
#include "Renderer/OrthographicCamera.hpp"
#include "OpenGL/Shader.hpp"


namespace Editor {

class SceneCamera;
extern const char* SCENE_CAMERA_TYPES[2];

class SceneCameraInspector: public Inspector {
public:
    SceneCameraInspector();
    void Draw() override;
    
    void SetContext(SceneCamera* context) { m_Context = context; };
private:
    SceneCamera* m_Context = nullptr;
    ImGuiInputSettings m_PositionSettings;
    ImGuiInputSettings m_RrotationSettings;
    ImGuiInputSettings m_ScaleSettings;
};


class SceneCamera {
public:
    enum CameraType {
        Otrhographic = 0,
        Perspective
    };
    
    SceneCamera(CameraType type = Otrhographic);
    ~SceneCamera();
    
    void OnUpdate();
    void Upload(const Core::Ref<OpenGL::Shader>& shader);
    
private:
    friend SceneCameraInspector;
    Renderer::OrthographicCamera m_OrthoCam;
    CameraType m_Type;
    
    TransformComponent m_Transform;
};

}

#endif /* SceneCamera_hpp */
