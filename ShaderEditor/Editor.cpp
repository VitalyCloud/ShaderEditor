//
//  Editor.cpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 20.04.2022.
//

#include "Core/Core.hpp"

namespace Editor {

class EditorLayer: public Engine::Layer {
public:
    virtual void OnUIRender() override {
        ImGui::Begin("Hello");
        ImGui::Button("Button");
        ImGui::End();
        
        ImGui::ShowDemoWindow();
    }
    
    virtual void OnEvent(Engine::Event& event) override {
        std::cout << event.GetName() << std::endl;
    }
};

}

Engine::Application* Engine::CreateApplication(int argc, char** argv) {
    Engine::ApplicationSpecification spec;
    spec.Name = "ShaderEditor";
    
    Engine::Application* app = new Engine::Application(spec);
    app->PushLayer<Editor::EditorLayer>();
 
    return app;
}
