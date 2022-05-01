//
//  Editor.cpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 20.04.2022.
//
#include "pch.h"
#include "Core/Core.hpp"
#include "OpenGL/OpenGL.hpp"

#include "Editor/ViewportPanel.hpp"
#include "Editor/InspectorPanel.hpp"

#include "Editor/VertexArrayView.hpp"

namespace Editor {

class EditorLayer: public Core::Layer {
public:
    
    EditorLayer() {}
    
    virtual void OnEvent(Core::Event& event) override {
        
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
            if (ImGui::MenuItem("Inspector", nullptr, m_ShowInspector))
                m_ShowInspector = !m_ShowInspector;
            
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
        
    }
    
    virtual void OnAttach() override {
        OpenGL::FramebufferSpecification spec;
        spec.Width = 800;
        spec.Height = 600;
        m_Framebuffer = Core::CreateRef<OpenGL::Framebuffer>(spec);
        m_ViewportPanel.SetFramebuffer(m_Framebuffer);
        Core::Application::Get().SetMenubarCallback(std::bind(&EditorLayer::OnMainMenuBar, this));
    
        m_VertexArrayContainer = Core::CreateRef<VertexArrayContainer>();
    }
    
    virtual void OnUpdate() override {
        m_Framebuffer->Bind();
        OpenGL::RenderCommand::SetClearColor(glm::vec4(0.3));
        OpenGL::RenderCommand::Clear();
        
        m_Framebuffer->Unbind();
    }
    
    virtual void OnUIRender() override {
        if(m_ShowViewport)
            m_ViewportPanel.Draw("Viewport", &m_ShowViewport);
        if(m_ShowInspector)
            m_InspectorPanel.Draw("Inspector", &m_ShowInspector);
    
        ImGui::Begin("VA Text");
        m_VertexArrayView.SetContext(m_VertexArrayContainer);
        m_VertexArrayView.Draw();
        ImGui::End();
    
    }
        
        
    
private:
    ViewportPanel m_ViewportPanel;
    bool m_ShowViewport = true;
    InspectorPanel m_InspectorPanel;
    bool m_ShowInspector = true;
    
    VertexArrayView m_VertexArrayView;
    Core::Ref<VertexArrayContainer> m_VertexArrayContainer = nullptr;
    
    Core::Ref<OpenGL::Framebuffer> m_Framebuffer = nullptr;
};

}

Core::Application* Core::CreateApplication(int argc, char** argv) {
    Core::ApplicationSpecification spec;
    spec.Name = "ShaderEditor";
    
    Core::Application* app = new Core::Application(spec);
    app->PushLayer<Editor::EditorLayer>();
    
    return app;
}
