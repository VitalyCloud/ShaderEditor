//
//  Editor.cpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 20.04.2022.
//

#include "Core/Application.hpp"
#include "Core/EntryPoint.hpp"
#include "imgui.h"

namespace Editor {

class EditorLayer: public Layer {
public:
    virtual void OnUIRender() override {
        ImGui::Begin("Hello");
        ImGui::Button("Button");
        ImGui::End();
        
        ImGui::ShowDemoWindow();
    }
};

Application* CreateApplication(int argc, char** argv) {
    ApplicationSpecification spec;
    spec.Name = "ShaderEditor";
    
    Application* app = new Editor::Application(spec);
    app->PushLayer<EditorLayer>();
 
    return app;
}

}
