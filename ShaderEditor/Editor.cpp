//
//  Editor.cpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 20.04.2022.
//

#include "Core/Core.hpp"
#include "OpenGL/OpenGL.h"
#include "Renderer/Renderer.hpp"

namespace Editor {

class EditorLayer: public Engine::Layer {
public:
    
    virtual void OnAttach() override {
        OpenGL::FramebufferSpecification spec;
        spec.Width = 800;
        spec.Height = 600;
        m_Framebuffer = Engine::CreateRef<OpenGL::Framebuffer>(spec);
        
        float verticies[3*2*3] = {
            -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
            0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
            0.0f, -0.5f, 0.0f, 0.0f, 1.0f
        };
        
        uint32_t indicies[3] = {
            0, 1, 2
        };
        
        auto vertexBuffer = Engine::CreateRef<OpenGL::VertexBuffer>(verticies, sizeof(verticies));
        vertexBuffer->SetLayout({
            { OpenGL::ShaderDataType::Float2, "a_Position" },
            { OpenGL::ShaderDataType::Float3, "a_Color" },
        });
        
        auto indexBuffer = Engine::CreateRef<OpenGL::IndexBuffer>(indicies, sizeof(indicies) / sizeof(uint32_t));
        
        m_VertexArray = Engine::CreateRef<OpenGL::VertexArray>();
        m_VertexArray->AddVertexBuffer(vertexBuffer);
        m_VertexArray->SetIndexBuffer(indexBuffer);
        
        
        std::string vertexSrc = R"(
            #version 330 core
            
            layout(location = 0) in vec2 a_Position;
            layout(location = 1) in vec3 a_Color;
        
            out vec3 v_Color;
            void main()
            {
                v_Color = a_Color;
                gl_Position = vec4(a_Position, 1.0f, 1.0f);
            }
        )";
        
        std::string fragmentSrc = R"(
            #version 330 core
            layout(location = 0) out vec4 color;
            in vec3 v_Color;
            void main()
            {
                color = vec4(v_Color, 1.0f);
            }
        )";
        
        m_Shader = Engine::CreateRef<OpenGL::Shader>(vertexSrc, fragmentSrc);
    }
    
    virtual void OnUpdate() override {
        m_Framebuffer->Bind();
        
        OpenGL::RenderCommand::SetClearColor(m_ClearColor);
        OpenGL::RenderCommand::Clear();
        
        Renderer::BeginScene();
        Renderer::Submit(m_Shader, m_VertexArray);
        Renderer::EndScene();
        
        
        
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
        
    }
    
private:
    Engine::Ref<OpenGL::Framebuffer> m_Framebuffer;
    Engine::Ref<OpenGL::VertexArray> m_VertexArray;
    Engine::Ref<OpenGL::Shader> m_Shader;
    
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
