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

#include "Editor/UniformPanel.hpp"
#include "Editor/PipelinePanel.hpp"

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
            if (ImGui::MenuItem("Pipeline", nullptr, m_ShowPipeline))
                m_ShowPipeline = !m_ShowPipeline;
            if (ImGui::MenuItem("Inspector", nullptr, m_ShowInspector))
                m_ShowInspector = !m_ShowInspector;
            if (ImGui::MenuItem("Uniforms", nullptr, m_ShowUniform))
                m_ShowUniform = !m_ShowUniform;
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
        
    }
    
    
    
    virtual void OnUpdate() override {
        m_Camera.SetPostion(m_CameraPostion);
        
        m_Framebuffer->Bind();
        
        OpenGL::RenderCommand::SetClearColor(m_ClearColor);
        OpenGL::RenderCommand::Clear();
        
        m_Pipeline.OnUpdate();
        
        m_Framebuffer->Unbind();
    }
    
    virtual void OnUIRender() override {
        if(m_ShowViewport)
            m_Viewport.Draw("Viewport", &m_ShowViewport);
        if(m_ShowPipeline)
            m_Pipeline.Draw("Pipeline", &m_ShowPipeline);
        if(m_ShowInspector)
            m_Pipeline.DrawInspector("Inspector", &m_ShowInspector);
        if(m_ShowUniform)
            m_Uniform.Draw("Uniforms", &m_ShowUniform);
        if(m_ShowTextEditor)
            m_TextEditor.Draw("Text Editor", &m_ShowTextEditor);
        
        
        if(m_ShowDebug) {
            ImGui::Begin("Debug", &m_ShowDebug);
            ImGui::Text("Clear color:");
            ImGui::SameLine();
            ImGui::ColorEdit4("##ClearColorInput", &m_ClearColor.x);
            ImGui::Text("Camera position:");
            ImGui::SameLine();
            ImGui::DragFloat3("##CameraPositionInput", &m_CameraPostion.x, 0.1, -10, 10);
            ImGui::End();
        }
        if(m_ShowImGuiDemo)
            ImGui::ShowDemoWindow(&m_ShowImGuiDemo);
    }
    
private:
    ViewportPanel m_Viewport;
    TextEditorPanel m_TextEditor;
    UniformPanel m_Uniform;
    PipelinePanel m_Pipeline;
    
    bool m_ShowViewport = true;
    bool m_ShowPipeline = true;
    bool m_ShowInspector = true;
    bool m_ShowUniform = true;
    bool m_ShowTextEditor = true;
    
    bool m_ShowImGuiDemo = false;
    bool m_ShowDebug = false;
    
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
