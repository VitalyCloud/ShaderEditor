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
#include "Editor/VertexView.hpp"
#include "Editor/UniformView.hpp"
#include "Editor/IndexView.hpp"

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
    
        m_VertexContainer = Core::CreateRef<VertexBufferContainer>();
        m_UniformContainer = Core::CreateRef<UniformBufferContainer>();
        m_IndexContainer = Core::CreateRef<IndexBufferContainer>();
        m_IndexContainer->GetData().push_back(0);
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
        
        ImGui::Begin("VertexTest");
        m_VertexView.SetContext(m_VertexContainer);
        m_VertexView.Draw();
        ImGui::End();
        
        ImGui::Begin("UniformTest");
        m_UniformView.SetContext(m_UniformContainer);
        m_UniformView.Draw();
        ImGui::End();
        
        ImGui::Begin("IndexTest");
        m_IndexView.SetContext(m_IndexContainer);
        m_IndexView.Draw();
        ImGui::End();
    }
    
private:
    ViewportPanel m_ViewportPanel;
    bool m_ShowViewport = true;
    InspectorPanel m_InspectorPanel;
    bool m_ShowInspector = true;
    
    VertexView m_VertexView;
    UniformView m_UniformView;
    IndexView m_IndexView;
    Core::Ref<VertexBufferContainer> m_VertexContainer;
    Core::Ref<UniformBufferContainer> m_UniformContainer;
    Core::Ref<IndexBufferContainer> m_IndexContainer;
    
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
