//
//  Mesh.cpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 01.05.2022.
//
#include "pch.h"
#include "Mesh.hpp"
#include "ImGuiHelper.hpp"
#include "OpenGL/RenderCommand.hpp"

namespace Editor {

Mesh::Mesh(const std::string& title)
: m_Title(title){
    m_VertexArrayContainer = Core::CreateRef<VertexArrayContainer>();
    m_VertexArrayContainer->AddVertexBuffer();
}

Mesh::Mesh(DefaultMesh::MeshType mesh, const std::string& title)
: m_Title(title) {
    
    switch (mesh) {
        case DefaultMesh::Triangle: {
            m_VertexArrayContainer = DefaultMesh::CreateTriangle();
            if(m_Title.empty())
                m_Title = "Triangle";
            break;
        }
            
        case DefaultMesh::Quad:
            m_VertexArrayContainer = DefaultMesh::CreateQuad();
            if(m_Title.empty())
                m_Title = "Quad";
            break;
        default:
            break;
    }
}

Mesh::~Mesh() {

}

void Mesh::OnUpdate(const Core::Ref<OpenGL::Shader>& shader) {
    auto va = m_VertexArrayContainer->GetVA();
    if(va != nullptr) {
        shader->Bind();
        va->Bind();
        OpenGL::RenderCommand::DrawIndexed(va);
    }
}

void MeshInspector::Draw() {
    if(m_Context == nullptr) return;
    
    ImGui::InputText("Title##MeshTitle", &m_Context->m_Title);
    m_VertexArrayView.SetContext(m_Context->m_VertexArrayContainer);
    m_VertexArrayView.Draw();
}

Core::Ref<VertexArrayContainer> DefaultMesh::CreateTriangle() {
    auto va = Core::CreateRef<VertexArrayContainer>();
    auto vb = Core::CreateRef<VertexBufferContainer>();
    
    vb->PushLayoutElement({OpenGL::ShaderDataType::Float2, "Pos"});
    vb->PushLayoutElement({OpenGL::ShaderDataType::Float3, "Color"});
    
    struct Vertex {
        glm::vec2 pos;
        glm::vec3 color;
    };
    
    Vertex v1 = {{-0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}};
    Vertex v2 = {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}};
    Vertex v3 = {{0.0f, -0.5f}, {0.0f, 0.0f, 1.0f}};
    
    vb->PushVertex((VertexBufferContainer::VertexData*)&v1, sizeof(Vertex));
    vb->PushVertex((VertexBufferContainer::VertexData*)&v2, sizeof(Vertex));
    vb->PushVertex((VertexBufferContainer::VertexData*)&v3, sizeof(Vertex));
    
    vb->UpdateVertexBufferIfNeeded();
    
    va->AddVertexBuffer(vb);
    va->GetIndexConteiner()->GetData() = {0, 1, 2};
    return va;
}

Core::Ref<VertexArrayContainer> DefaultMesh::CreateQuad() {
    auto va = Core::CreateRef<VertexArrayContainer>();
    auto vb = Core::CreateRef<VertexBufferContainer>();
    
    vb->PushLayoutElement({OpenGL::ShaderDataType::Float2, "Pos"});
    
    glm::vec2 v1 = {-1.0f, 1.0f};
    glm::vec2 v2 = {1.0f, 1.0f};
    glm::vec2 v3 = {1.0f, -1.0f};
    glm::vec2 v4 = {-1.0f, -1.0f};
    
    vb->PushVertex((VertexBufferContainer::VertexData*)&v1, sizeof(glm::vec2));
    vb->PushVertex((VertexBufferContainer::VertexData*)&v2, sizeof(glm::vec2));
    vb->PushVertex((VertexBufferContainer::VertexData*)&v3, sizeof(glm::vec2));
    vb->PushVertex((VertexBufferContainer::VertexData*)&v4, sizeof(glm::vec2));
    
    vb->UpdateVertexBufferIfNeeded();
    
    va->AddVertexBuffer(vb);
    va->GetIndexConteiner()->GetData() = {0, 1, 2, 2, 0, 3};
    return va;
}


}
