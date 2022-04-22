//
//  Editor.cpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 20.04.2022.
//

#include "Core/Core.hpp"
#include "OpenGL/OpenGL.h"
#include "Renderer/Renderer.hpp"
#include "Editor/ViewportPanel.hpp"

namespace Editor {

// TODO: [x] Assert
// TODO: [x] Logging
// TODO: [ ] Camera

// TODO: [ ] Main Menu Bar

// TODO: [ ] TextEditor Panel
// TODO: [ ] Pipeline Panel
// TODO: [ ] Inspector Panel

// TODO: Textures

class EditorLayer: public Engine::Layer {
public:
    
    virtual void OnAttach() override {
        OpenGL::FramebufferSpecification spec;
        spec.Width = 800;
        spec.Height = 600;
        m_Framebuffer = Engine::CreateRef<OpenGL::Framebuffer>(spec);
        m_Viewport.SetTexture((ImTextureID)(uintptr_t)m_Framebuffer->GetColorAttachmentRendererID());
        m_Viewport.SetResizeCallback(std::bind(&EditorLayer::OnViewportResize,
                                               this, std::placeholders::_1, std::placeholders::_2));
        
        float positions[3*3] = {
            -0.5f, 0.5f,
            0.5f, 0.5f,
            0.0f, -0.5f
        };
        
        float colors[3*3] = {
            1.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 1.0f
        };
        
        uint32_t indicies[3] = {
            0, 1, 2
        };
        
        auto postionBuffer = Engine::CreateRef<OpenGL::VertexBuffer>(positions, sizeof(positions));
        postionBuffer->SetLayout({
            { OpenGL::ShaderDataType::Float2, "a_Position" },
            
        });
        
        auto colorBuffer = Engine::CreateRef<OpenGL::VertexBuffer>(colors, sizeof(colors));
        colorBuffer->SetLayout({
            { OpenGL::ShaderDataType::Float3, "a_Color" },
        });
        
        auto indexBuffer = Engine::CreateRef<OpenGL::IndexBuffer>(indicies, sizeof(indicies) / sizeof(uint32_t));
        
        m_VertexArray = Engine::CreateRef<OpenGL::VertexArray>();
        m_VertexArray->AddVertexBuffer(postionBuffer);
        m_VertexArray->AddVertexBuffer(colorBuffer);
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
        
        m_Viewport.Draw("Viewport");
        
        ImGui::ColorEdit4("Clear Color", &m_ClearColor.x);
        
        ImGui::End();
        
        ImGui::ShowDemoWindow();
    }
    
    virtual void OnEvent(Engine::Event& event) override {
        
    }
    
    void OnViewportResize(uint32_t width, uint32_t height) {
        EN_INFO("Viewport resized: {0}x{1}", width, height);
    }
    
private:
    ViewportPanel m_Viewport;
    
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
