//
//  Editor.cpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 20.04.2022.
//

#include "Core/Core.hpp"
#include "OpenGL/Framebuffer.hpp"
#include "glad/glad.h"

namespace Editor {

class EditorLayer: public Engine::Layer {
public:
    
    virtual void OnAttach() override {
        OpenGL::FramebufferSpecification spec;
        spec.Width = 800;
        spec.Height = 600;
        m_Framebuffer = Engine::CreateRef<OpenGL::Framebuffer>(spec);
    }
    
    virtual void OnUpdate() override {
        m_Framebuffer->Bind();
        
        glClearColor(m_ClearColor.x, m_ClearColor.y, m_ClearColor.z, m_ClearColor.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        m_Framebuffer->Unbind();
    }
    
    virtual void OnUIRender() override {
        ImGui::Begin("Hello");
        
        ImGui::Image((ImTextureID*)m_Framebuffer->GetColorAttachmentRendererID(), ImVec2(800, 600), ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
        
        
        ImGui::ColorEdit4("Clear Color", &m_ClearColor.x);
        
        ImGui::End();
        
        ImGui::ShowDemoWindow();
    }
    
    virtual void OnEvent(Engine::Event& event) override {
        std::cout << event.GetName() << std::endl;
    }
    
private:
    Engine::Ref<OpenGL::Framebuffer> m_Framebuffer;
    glm::vec4 m_ClearColor = glm::vec4(0.3f);
};

}

Engine::Application* Engine::CreateApplication(int argc, char** argv) {
    Engine::ApplicationSpecification spec;
    spec.Name = "ShaderEditor";
    
    Engine::Application* app = new Engine::Application(spec);
    app->PushLayer<Editor::EditorLayer>();
 
    return app;
}
