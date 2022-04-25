//
//  Pipeline.cpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 25.04.2022.
//

#include "Core/pch.h"
#include "Pipeline.hpp"

#include "imgui.h"
#include "ImGuiHelper.h"

namespace Editor {

ShaderPass::ShaderPass(const std::string& title) : m_Title(title) {
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

    m_Shader = Core::CreateRef<OpenGL::Shader>(vertexSrc, fragmentSrc);
}

ShaderPass::~ShaderPass() {
    
}

void ShaderPass::OnUpdate() {
    
    m_Shader->Bind();
    m_VertexArray.GetOpenGLVA()->Bind();
//    OpenGL::RenderCommand::Draw(m_VertexArray.GetVertexCount());
    OpenGL::RenderCommand::DrawIndexed(m_VertexArray.GetOpenGLVA());
}

Pipeline::Pipeline() {
    m_ShaderPassses.push_back(Core::CreateRef<ShaderPass>("1"));
}

Pipeline::~Pipeline() {
    
}

void Pipeline::OnUpdate() {
    for(auto& shaderPass: m_ShaderPassses)
        shaderPass->OnUpdate();
}

void Pipeline::Draw(const char* title, bool* p_open) {
    if (!ImGui::Begin(title, p_open)) {
        ImGui::End();
        return;
    }
    
    for(int i=0; i<m_ShaderPassses.size(); i++) {
        auto& shaderPass = m_ShaderPassses[i];
        auto title = fmt::format("{0}##{1}", shaderPass->GetTitle(), i);
        ImGui::Selectable(title.c_str(), m_SelectedShaderPasss == shaderPass);
        if (ImGui::IsItemClicked()) {
            m_SelectedShaderPasss = shaderPass;
        }
    }
    
    DrawInspector();
   
    ImGui::End();
}

void Pipeline::DrawInspector() {
    ImGui::Begin("Inspector");
    
    if(m_SelectedShaderPasss != nullptr) {
        ImGui::Text("Title"); ImGui::SameLine();
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
        ImGui::InputText("##inputTitle", &m_SelectedShaderPasss->GetTitle());
        ImGui::PopItemWidth();
        
        m_SelectedShaderPasss->GetVA().Draw();
    }
    
    
    ImGui::End();
}


VertexArray::VertexArray() {
    AddVertexBuffer();
    
    auto vb = m_VertexBuffers[0];
    
    vb->ChangeElementType(0, OpenGL::ShaderDataType::Float2);
    vb->PushLayoutElement({OpenGL::ShaderDataType::Float3, "Color"});
    vb->PushVertex();
    vb->PushVertex();

    float* pos0 = (float*)vb->GetVertexComponent(0, 0);
    *(pos0+0) = -0.5;
    *(pos0+1) = 0.5;

    float* pos1 = (float*)vb->GetVertexComponent(1, 0);
    *(pos1+0) = 0.5;
    *(pos1+1) = 0.5;

    float* pos2 = (float*)vb->GetVertexComponent(2, 0);
    *(pos2+0) = 0.0;
    *(pos2+1) = -0.5;
    
    float* color0 = (float*)vb->GetVertexComponent(0, 1);
    *(color0+0) = 1.0;
    *(color0+1) = 0.0;
    *(color0+2) = 0.0;

    float* color1 = (float*)vb->GetVertexComponent(1, 1);
    *(color1+0) = 0.0;
    *(color1+1) = 1.0;
    *(color1+2) = 0.0;

    float* color2 = (float*)vb->GetVertexComponent(2, 1);
    *(color2+0) = 0.0;
    *(color2+1) = 0.0;
    *(color2+2) = 1.0;
//
    
//    m_Indicies.resize(1, 0);
    m_Indicies = {0, 1, 2};
}

VertexArray::~VertexArray() {
    
}

void VertexArray::InvalidateVertexArray() {
    EN_INFO("Invalidating VBs Changes");
    m_VertexArrayInternal = Core::CreateRef<OpenGL::VertexArray>();
    for(auto buffer: m_VertexBuffers) {
        auto vb = buffer->CreateOpenGLVertexBuffer();
        m_VertexArrayInternal->AddVertexBuffer(vb);
    }
    auto indexBuffer = Core::CreateRef<OpenGL::IndexBuffer>(m_Indicies.data(), m_Indicies.size());
    m_VertexArrayInternal->SetIndexBuffer(indexBuffer);
}

void VertexArray::AddVertexBuffer() {
    EN_INFO("AddVertexBuffer");
    auto newBuffer = Core::CreateRef<VertexBuffer>();
    newBuffer->SetChanged(false);
    m_VertexBuffers.push_back(newBuffer);
    
    InvalidateVertexArray();
}

void VertexArray::RemoveVertexBuffer(int index) {
    EN_INFO("RemoveVertexBuffer");
    m_VertexBuffers.erase(m_VertexBuffers.begin() + index);
    InvalidateVertexArray();
}

void VertexArray::InvalidateChanges(int index) {
    EN_INFO("InvalidateChanges");
    InvalidateVertexArray();
}

void VertexArray::OnUpdate() {
    
}
                             

void VertexArray::Draw() {
    ImGui::Separator();
    m_VertexBufferChanged = false;
    for(int i=0; i<m_VertexBuffers.size(); i++) {
        auto title = fmt::format("Buffer #{0}", i);
        bool close = true;
        
        bool open = ImGui::CollapsingHeader(title.c_str(), &close);
        if(ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
            ImGui::OpenPopup("BufferContext");
        }
        
        if(open) {
            ImGui::PushID(i);
            m_VertexView.SetContext(m_VertexBuffers[i]);
            m_VertexView.Draw();
            ImGui::PopID();
        }
        
        if(!close && m_VertexBuffers.size() > 1) {
            RemoveVertexBuffer(i);
        }
        if(m_VertexBuffers[i]->IsChanged()) {
            InvalidateChanges(i);
            m_VertexBuffers[i]->SetChanged(false);
        }
            
    }
   
    if(ImGui::BeginPopup("BufferContext")) {
        if(ImGui::Selectable("Add Buffer")) {
            AddVertexBuffer();
        }
        ImGui::EndPopup();
    }
    
    if(ImGui::CollapsingHeader("Index Buffer")) {
        m_IndexView.Draw(m_Indicies);
        if(m_IndexView.IsChanged()) {
            InvalidateChanges(0);
        }
    }
}

}
