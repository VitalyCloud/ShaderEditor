//
//  Editor.cpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 20.04.2022.
//

#include "Core/Core.hpp"
#include "OpenGL/OpenGL.hpp"
#include "Renderer/Renderer.hpp"
#include "Editor/ViewportPanel.hpp"
#include "Editor/TextEditorPanel.hpp"

namespace Editor {

// TODO: [x] Assert
// TODO: [x] Logging
// TODO: [x] Camera

// TODO: [x] Main Menu Bar

// TODO: [x] TextEditor Panel
// TODO: [ ] Pipeline Panel
// TODO: [ ] Inspector Panel

// TODO: [ ] Textures

class EditorLayer: public Engine::Layer {
public:
    
    EditorLayer() : m_Camera(-1.0f, 1.0f, -1.0f, 1.0f) {}
    
    virtual void OnAttach() override {
        OpenGL::FramebufferSpecification spec;
        spec.Width = 800;
        spec.Height = 600;
        m_Framebuffer = Engine::CreateRef<OpenGL::Framebuffer>(spec);
        m_Viewport.SetTexture((ImTextureID)(uintptr_t)m_Framebuffer->GetColorAttachmentRendererID());
        m_Viewport.SetResizeCallback(std::bind(&EditorLayer::OnViewportResize,
                                               this, std::placeholders::_1, std::placeholders::_2));
        
        Engine::Application::Get().SetMenubarCallback([this]() {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Exit"))
                {
                    Engine::Application::Get().Close();
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Windows")) {
                if (ImGui::MenuItem("Viewport", nullptr, m_ShowViewport))
                    m_ShowViewport = !m_ShowViewport;
                if (ImGui::MenuItem("Text Editor", nullptr, m_ShowTextEditor))
                    m_ShowTextEditor = !m_ShowTextEditor;
                if (ImGui::MenuItem("ImGui Demo", nullptr, m_ShowImGuiDemo))
                    m_ShowImGuiDemo = !m_ShowImGuiDemo;
                ImGui::EndMenu();
            }
            
            if (ImGui::BeginMenu("Settings")) {
                bool VSync = Engine::Application::Get().IsVSync();
                if (ImGui::MenuItem("VSync", nullptr, &VSync))
                    Engine::Application::Get().SetVSync(!VSync);
                ImGui::EndMenu();
            }
        });
        
        float a = 0.5f;
        
        float positions[3*2] = {
            -a, a,
            a, a,
            0.0f, -a
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
        
            uniform mat4 u_ViewProjection;
        
            out vec3 v_Color;
            void main()
            {
                v_Color = a_Color;
                gl_Position = u_ViewProjection * vec4(a_Position, 1.0f, 1.0f);
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
        m_Camera.SetPostion(m_CameraPostion);
        
        m_Framebuffer->Bind();
        
        OpenGL::RenderCommand::SetClearColor(m_ClearColor);
        OpenGL::RenderCommand::Clear();
        
        Renderer::BeginScene(m_Camera);
        Renderer::Submit(m_Shader, m_VertexArray);
        Renderer::EndScene();
        
        
        m_Framebuffer->Unbind();
    }
    
    virtual void OnUIRender() override {
        ImGui::Begin("Hello");
        ImGui::ColorEdit4("Clear Color", &m_ClearColor.x);
        ImGui::DragFloat3("Camera: ", &m_CameraPostion.x, 0.1, -10, 10);
        ImGui::End();
        
        if(m_ShowViewport)
            m_Viewport.Draw("Viewport", &m_ShowViewport);
        
        if(m_ShowTextEditor)
            m_TextEditor.Draw("Text Editor", &m_ShowTextEditor);
        
        if(m_ShowImGuiDemo)
            ImGui::ShowDemoWindow(&m_ShowImGuiDemo);
    }
    
    virtual void OnEvent(Engine::Event& event) override {
        m_TextEditor.OnEvent(event);
    }
    
    void OnViewportResize(uint32_t width, uint32_t height) {
        EN_INFO("Viewport resized: {0}x{1}", width, height);
        
        float A = (float)width / (float)height;
        
        float x = 1;
        float y = 1;
        x *= A;
    
        m_Camera.SetCamera(-x, x, -y, y);
        
    }
    
private:
    ViewportPanel m_Viewport;
    TextEditorPanel m_TextEditor;
    
    bool m_ShowViewport = true;
    bool m_ShowTextEditor = true;
    bool m_ShowImGuiDemo = false;
    
    Engine::Ref<OpenGL::Framebuffer> m_Framebuffer;
    Engine::Ref<OpenGL::VertexArray> m_VertexArray;
    Engine::Ref<OpenGL::Shader> m_Shader;
    Renderer::OrthographicCamera m_Camera;
    
    glm::vec3 m_CameraPostion = glm::vec3(0.0);
    glm::vec4 m_ClearColor = glm::vec4(0.3f);
};

}

Engine::Application* Engine::CreateApplication(int argc, char** argv) {
    Engine::ApplicationSpecification spec;
    spec.Name = "ShaderEditor";
    
    Engine::Application* app = new Engine::Application(spec);
    app->PushLayer<Editor::EditorLayer>();
 
//    app->SetMenubarCallback([app]()
//    {
//        if (ImGui::BeginMenu("File"))
//        {
//            if (ImGui::MenuItem("Exit"))
//            {
//                app->Close();
//            }
//            ImGui::EndMenu();
//        }
//    });
    
    return app;
}
