//
//  Mesh.cpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 27.04.2022.
//

#include "Core/pch.h"
#include "Mesh.hpp"

#include "ImGuiHelper.h"

namespace Editor {

Mesh::Mesh(const std::string& title) : m_Title(title) {
    m_Indicies = Core::CreateRef<std::vector<uint32_t>>();
    m_Indicies = {0};
    m_VertexBuffers.push_back(Core::CreateRef<VertexBufferConteiner>());
    m_VertexBuffers[0]->PushLayoutElement({OpenGL::ShaderDataType::Float, "Element"});
    m_VertexBuffers[0]->PushVertex();
}

Mesh::Mesh(DefaultMesh defaultMesh, const std::string& title) : m_Title(title) {
    m_Indicies = Core::CreateRef<std::vector<uint32_t>>();
    PopulateDefaultMesh(defaultMesh);
}

Mesh::~Mesh() {
    
}

void Mesh::Draw(const Core::Ref<OpenGL::Shader>& shader) {
    if(m_VertexArray != nullptr && m_VertexArray->GetVertexBuffers().size() > 0) {
        
        // Texture binding
        for(int i=0; i<m_Textures.size(); i++) {
            auto& texture = m_Textures[i];
            texture->Bind(i);
            if(!texture->GetTitle().empty()) {
                shader->SetInt(texture->GetTitle(), i);
            }
        }
        
        // Vertex Array
        m_VertexArray->Bind();
        
        // Drawing
        if(m_DrawIndexed)
            OpenGL::RenderCommand::DrawIndexed(m_VertexArray);
        else
            OpenGL::RenderCommand::Draw(m_VertexBuffers[0]->VertexCount());
    }
}

void Mesh::InvalidateVertexArray() {
    EN_INFO("Invalidating Vertex Array Changes");
        
    m_VertexArray = Core::CreateRef<OpenGL::VertexArray>();
    for(auto buffer: m_VertexBuffers) {
        if(buffer->GetLayoutElements().size() > 0) {
            auto vb = buffer->CreateOpenGLVertexBuffer();
            m_VertexArray->AddVertexBuffer(vb);
        }
    }
    auto indexBuffer = Core::CreateRef<OpenGL::IndexBuffer>(m_Indicies->data(), m_Indicies->size());
    m_VertexArray->SetIndexBuffer(indexBuffer);
}

void Mesh::AddVertexBuffer() {
    EN_INFO("Add new vertex buffer");
    auto newBuffer = Core::CreateRef<VertexBufferConteiner>();
    newBuffer->SetChanged(false);
    m_VertexBuffers.push_back(newBuffer);
    
    InvalidateVertexArray();
}

void Mesh::RemoveVertexBuffer(int index) {
    EN_INFO("Remove vertex buffer at index {0}", index);
    m_VertexBuffers.erase(m_VertexBuffers.begin() + index);
    InvalidateVertexArray();
}

void MeshInspector::Draw() {
    if(m_Context == nullptr)
        return;
    
    ImGui::InputText("Title##MeshTitle", &m_Context->m_Title);
    
    for(int i=0; i<m_Context->m_VertexBuffers.size(); i++) {
        auto title = fmt::format("Buffer #{0}", i);
        bool close = true;
        
        bool open = false;
        if(m_Context->m_VertexBuffers.size() == 1)
            open = ImGui::CollapsingHeader(title.c_str());
        else
            open = ImGui::CollapsingHeader(title.c_str(), &close);
        
        if(ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
            ImGui::OpenPopup("BufferContext");
        }
        
        if(open) {
            ImGui::PushID(i);
            m_VertexView.SetContext(m_Context->m_VertexBuffers[i]);
            m_VertexView.Draw();
            ImGui::PopID();
        }
        
        if(!close && m_Context->m_VertexBuffers.size() > 1) {
            m_Context->RemoveVertexBuffer(i);
        }
        if(m_Context->m_VertexBuffers[i]->IsChanged()) {
            m_Context->InvalidateVertexArray();
            m_Context->m_VertexBuffers[i]->SetChanged(false);
        }
            
    }
   
    if(ImGui::BeginPopup("BufferContext")) {
        if(ImGui::Selectable("Add Buffer")) {
            m_Context->AddVertexBuffer();
        }
        ImGui::EndPopup();
    }
    
    if(ImGui::CollapsingHeader("Index Buffer")) {
        ImGui::Checkbox("Draw Indexed", &m_Context->m_DrawIndexed);
        m_IndexView.SetContext(m_Context->m_Indicies);
        m_IndexView.Draw();
        if(m_IndexView.IsChanged()) {
            m_Context->InvalidateVertexArray();
        }
    }
    
    if(ImGui::CollapsingHeader("Textures")) {
        if(ImGui::BeginTable("MeshTextrues", 3)) {
            ImGui::TableSetupColumn("Slot", ImGuiTableColumnFlags_WidthFixed, 40);
            ImGui::TableSetupColumn("Title");
            ImGui::TableSetupColumn("Path");
            ImGui::TableHeadersRow();
            
            for(int i=0; i<m_Context->m_Textures.size(); i++) {
                auto& texture = m_Context->m_Textures[i];
                ImGui::PushID(i);
                ImGui::TableNextRow();
                
                // Slot
                ImGui::TableSetColumnIndex(0);
                if(ImGui::Button("x")) {
                    m_Context->m_Textures.erase(m_Context->m_Textures.begin() + i);
                }
                ImGui::SameLine();
                ImGui::Text("%d", i);
                
                // Title
                ImGui::TableSetColumnIndex(1);
                ImGui::PushItemWidth(ImGui::GetColumnWidth());
                ImGui::InputText("##textureTitle", &texture->GetTitle());
                ImGui::PopItemWidth();
                
                // Path
                ImGui::TableSetColumnIndex(2);
                ImGui::PushItemWidth(ImGui::GetColumnWidth());
                ImGui::InputText("##texturePath", &texture->GetFilePath(), ImGuiInputTextFlags_ReadOnly);
                ImGui::PopItemWidth();
                
                ImGui::PopID();
            }
            
            ImGui::EndTable();
        }
        
        if(ImGui::Button("+##AddTexture")) {
            auto path = Core::Utils::FileDialogs::OpenFile("");
            if(!path.empty()) {
                m_Context->m_Textures.push_back(Core::CreateRef<OpenGL::Texture>(path));
            }
        }
    }
    
}

void Mesh::PopulateDefaultMesh(DefaultMesh defaultMesh) {
    switch (defaultMesh) {
        case DefaultMesh::Triangle: {
            m_VertexBuffers.push_back(Core::CreateRef<VertexBufferConteiner>());
            auto vb = m_VertexBuffers[0];
            
            vb->PushLayoutElement({OpenGL::ShaderDataType::Float2, "Pos"});
            vb->PushLayoutElement({OpenGL::ShaderDataType::Float3, "Color"});
            vb->PushVertex();
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

            *m_Indicies = {0, 1, 2};
            
            m_DrawIndexed = true;
            
            if(m_Title.empty())
                m_Title = "Triangle";
    
            InvalidateVertexArray();
            
           
            break;
        }
        case DefaultMesh::Squad: {
            m_VertexBuffers.push_back(Core::CreateRef<VertexBufferConteiner>());
            auto vb = m_VertexBuffers[0];
            
            vb->PushLayoutElement({OpenGL::ShaderDataType::Float2, "Pos"});
            vb->PushVertex();vb->PushVertex();
            vb->PushVertex();vb->PushVertex();
            
            float* pos0 = (float*)vb->GetVertexComponent(0, 0);
            *(pos0+0) = -1.0;
            *(pos0+1) = 1.0;

            float* pos1 = (float*)vb->GetVertexComponent(1, 0);
            *(pos1+0) = 1.0;
            *(pos1+1) = 1.0;

            float* pos2 = (float*)vb->GetVertexComponent(2, 0);
            *(pos2+0) = 1.0;
            *(pos2+1) = -1.0;
            
            float* pos3 = (float*)vb->GetVertexComponent(3, 0);
            *(pos3+0) = -1.0;
            *(pos3+1) = -1.0;
            
            *m_Indicies = {0, 1, 2, 2, 0, 3};
            m_DrawIndexed = true;
            
            if(m_Title.empty())
                m_Title = "Squad";
            
            InvalidateVertexArray();
            
            break;
        }
        default:
            EN_ERROR("DefaultMesh is unknown");
            break;
    }
}

}
