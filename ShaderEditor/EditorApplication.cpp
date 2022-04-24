//
//  Editor.cpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 20.04.2022.
//
#include "Core/pch.h"
#include "Core/Core.hpp"
#include "OpenGL/OpenGL.hpp"
#include "Renderer/Renderer.hpp"
#include "Editor/ViewportPanel.hpp"
#include "Editor/TextEditorPanel.hpp"

#include "Editor/UniformView.hpp"
#include "Editor/VertexView.hpp"

namespace Editor {

// TODO: [x] Assert
// TODO: [x] Logging
// TODO: [x] Camera

// TODO: [x] Main Menu Bar

// TODO: [x] TextEditor Panel
// TODO: [ ] Pipeline Panel
// TODO: [ ] Inspector Panel

// TODO: [ ] Textures

class EditorLayer: public Core::Layer {
public:
    
    EditorLayer() : m_Camera(-1.0f, 1.0f, -1.0f, 1.0f) {}
    
    virtual void OnEvent(Core::Event& event) override {
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
    
    void OnMainMenuBar() {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Exit"))
            {
                Core::Application::Get().Close();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Window")) {
            if (ImGui::MenuItem("Viewport", nullptr, m_ShowViewport))
                m_ShowViewport = !m_ShowViewport;
            if (ImGui::MenuItem("Text Editor", nullptr, m_ShowTextEditor))
                m_ShowTextEditor = !m_ShowTextEditor;
            if (ImGui::MenuItem("ImGui Demo", nullptr, m_ShowImGuiDemo))
                m_ShowImGuiDemo = !m_ShowImGuiDemo;
            if (ImGui::MenuItem("Debug", nullptr, m_ShowDebug))
                m_ShowDebug = !m_ShowDebug;
            ImGui::EndMenu();
        }
        
        if (ImGui::BeginMenu("Settings")) {
            bool VSync = Core::Application::Get().IsVSync();
            if (ImGui::MenuItem("VSync", nullptr, VSync))
                Core::Application::Get().SetVSync(!VSync);
            
            if(ImGui::BeginMenu("Style")) {
                if (ImGui::MenuItem("Light"))
                    ImGui::StyleColorsLight();
                if (ImGui::MenuItem("Dark"))
                    ImGui::StyleColorsDark();
                if (ImGui::MenuItem("Classic"))
                    ImGui::StyleColorsClassic();
                
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }
        ImGui::Separator();
        {
            const float FramesPerSecond = ImGui::GetIO().Framerate;
            const float Frametime = 1000.0f / FramesPerSecond;
            const auto fps = fmt::format("{0:.2f} FPS ({1:.2f} ms)",
                FramesPerSecond, Frametime);
            auto size = ImGui::CalcTextSize(fps.c_str());
            ImGui::SameLine(0, ImGui::GetContentRegionAvail().x - size.x);
            ImGui::Text("%s", fps.c_str());
        }
    }
    
    virtual void OnAttach() override {
        OpenGL::FramebufferSpecification spec;
        spec.Width = 800;
        spec.Height = 600;
        m_Framebuffer = Core::CreateRef<OpenGL::Framebuffer>(spec);
        m_Viewport.SetTexture((ImTextureID)(uintptr_t)m_Framebuffer->GetColorAttachmentRendererID());
        m_Viewport.SetResizeCallback(std::bind(&EditorLayer::OnViewportResize,
                                               this, std::placeholders::_1, std::placeholders::_2));
        
        Core::Application::Get().SetMenubarCallback(std::bind(&EditorLayer::OnMainMenuBar, this));
        
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
        
        auto postionBuffer = Core::CreateRef<OpenGL::VertexBuffer>(positions, sizeof(positions));
        postionBuffer->SetLayout({
            { OpenGL::ShaderDataType::Float2, "a_Position" },
            
        });
        
        auto colorBuffer = Core::CreateRef<OpenGL::VertexBuffer>(colors, sizeof(colors));
        colorBuffer->SetLayout({
            { OpenGL::ShaderDataType::Float3, "a_Color" },
        });
        
        auto indexBuffer = Core::CreateRef<OpenGL::IndexBuffer>(indicies, sizeof(indicies) / sizeof(uint32_t));
        
        m_VertexArray = Core::CreateRef<OpenGL::VertexArray>();
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
        
        m_Shader = Core::CreateRef<OpenGL::Shader>(vertexSrc, fragmentSrc);
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
        if(m_ShowDebug) {
            ImGui::Begin("Debug", &m_ShowDebug);
            ImGui::Text("Clear color:");
            ImGui::SameLine();
            ImGui::ColorEdit4("##ClearColorInput", &m_ClearColor.x);
            ImGui::Text("Camera position:");
            ImGui::SameLine();
            ImGui::DragFloat3("##CameraPositionInput", &m_CameraPostion.x, 0.1, -10, 10);
            m_UniformView.Draw();
            ImGui::End();
        }
        
        if(m_ShowViewport)
            m_Viewport.Draw("Viewport", &m_ShowViewport);
        
        if(m_ShowTextEditor)
            m_TextEditor.Draw("Text Editor", &m_ShowTextEditor);
        
        if(m_ShowImGuiDemo)
            ImGui::ShowDemoWindow(&m_ShowImGuiDemo);
    }
    
private:
    ViewportPanel m_Viewport;
    TextEditorPanel m_TextEditor;
    UniformView m_UniformView;
    VertexView m_VertexView;
    
    bool m_ShowViewport = true;
    bool m_ShowTextEditor = true;
    bool m_ShowImGuiDemo = false;
    bool m_ShowDebug = true;
    
    Core::Ref<OpenGL::Framebuffer> m_Framebuffer;
    Core::Ref<OpenGL::VertexArray> m_VertexArray;
    Core::Ref<OpenGL::Shader> m_Shader;
    Renderer::OrthographicCamera m_Camera;
    
    glm::vec3 m_CameraPostion = glm::vec3(0.0);
    glm::vec4 m_ClearColor = glm::vec4(0.3f);
};

}

Core::Application* Core::CreateApplication(int argc, char** argv) {
    Core::ApplicationSpecification spec;
    spec.Name = "ShaderEditor";
    
    Core::Application* app = new Core::Application(spec);
    app->PushLayer<Editor::EditorLayer>();
    
    return app;
}
