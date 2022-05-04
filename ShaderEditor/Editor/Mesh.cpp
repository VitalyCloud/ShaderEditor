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
        case DefaultMesh::Cube:
            m_VertexArrayContainer = DefaultMesh::CreateCube();
            if(m_Title.empty())
                m_Title = "Cube";
            break;
        default:
            break;
    }
}

Mesh::~Mesh() {

}

void Mesh::OnUpdate(const Core::Ref<OpenGL::Shader>& shader) {
    auto va = m_VertexArrayContainer->GetVA();
    if(va != nullptr && va->GetVertexBuffers().size() > 0) {
        va->Bind();
        shader->SetMat4("u_Transform", m_Transform.GetTransform());
        if(m_UseIndexBuffer)
            OpenGL::RenderCommand::DrawIndexed(va, m_Topology);
        else {
            auto count = m_VertexArrayContainer->GetVertexConteiners()[0]->Count();
            OpenGL::RenderCommand::Draw(count, m_Topology);
        }
        
        va->Unbind();
    }
}

MeshInspector::MeshInspector() {
    ImGuiInputSettings settings;
    settings.Type = ImGuiWidgetType::Drag;
    settings.Speed = 0.050f;
    
    m_PositionSettings = settings;
    m_RrotationSettings = settings;
    m_ScaleSettings = settings;
}

//Points,
//
//Lines,
//LineStrip,
//LineLoop,
//
//Triangles,
//TriangleStrip,
//TrinagleFan

static const char* const s_Primitives[] = {
    "Points",
    "Lines",
    "Line Strip",
    "Line Loop",
    "Trinagles",
    "Triangle Strip",
    "Trinagle Fan"
};

void MeshInspector::Draw() {
    if(m_Context == nullptr) return;
    
    ImGui::PushID(1);
    ImGui::InputText("Title##MeshTitle", &m_Context->m_Title);
    m_VertexArrayView.SetContext(m_Context->m_VertexArrayContainer);
    m_VertexArrayView.Draw();
    ImGui::Checkbox("Use Index Buffer", &m_Context->m_UseIndexBuffer);

    ImGui::Text("Position"); ImGui::SameLine();
    ImGui::PushID("PositionInput");
    ImGui::DrawInputFloat(&m_Context->m_Transform.Translation.x, &m_PositionSettings, 3);
    ImGui::PopID();
    ImGui::SameLine();
    if(ImGui::Button(":##PositionSettings")) {
        m_PopupContext = &m_PositionSettings;
        ImGui::OpenPopup("InputSettings");
    }
    
    ImGui::Text("Rotation"); ImGui::SameLine();
    ImGui::PushID("RotationInput");
    ImGui::DrawInputFloat(&m_Context->m_Transform.Rotation.x, &m_RrotationSettings, 3);
    ImGui::PopID();
    ImGui::SameLine();
    if(ImGui::Button(":##RotationSettings")) {
        m_PopupContext = &m_RrotationSettings;
        ImGui::OpenPopup("InputSettings");
    }
    
    ImGui::Text("Scale"); ImGui::SameLine();
    ImGui::PushID("ScaleInput");
    ImGui::DrawInputFloat(&m_Context->m_Transform.Scale.x, &m_ScaleSettings, 3);
    ImGui::PopID();
    ImGui::SameLine();
    if(ImGui::Button(":##ScaleSettings")) {
        m_PopupContext = &m_ScaleSettings;
        ImGui::OpenPopup("InputSettings");
    }
    
    if(ImGui::BeginPopup("InputSettings") && m_PopupContext != nullptr) {
        ImGui::DrawInputSettingsView(OpenGL::ShaderDataType::Float3, m_PopupContext);
        ImGui::EndPopup();
    }
    
    ImGui::Text("Topology"); ImGui::SameLine();
    int selectedTopology = static_cast<int>(m_Context->m_Topology);
    if(ImGui::Combo("##TopologyType", &selectedTopology, s_Primitives, IM_ARRAYSIZE(s_Primitives)))
        m_Context->m_Topology = OpenGL::Primitive(selectedTopology);
    
    ImGui::PopID();
}

Core::Ref<VertexArrayContainer> DefaultMesh::CreateTriangle() {
    auto va = Core::CreateRef<VertexArrayContainer>();
    auto vb = Core::CreateRef<VertexBufferContainer>();
    auto ib = Core::CreateRef<IndexBufferContainer>();
    
    vb->PushLayoutElement({OpenGL::ShaderDataType::Float3, "Pos"});
    vb->PushLayoutElement({OpenGL::ShaderDataType::Float3, "Color"});
    
    struct Vertex {
        glm::vec3 pos;
        glm::vec3 color;
    };
    
    Vertex v1 = {{-0.5f, 0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}};
    Vertex v2 = {{0.5f, 0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}};
    Vertex v3 = {{0.0f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}};
    
    vb->PushVertex((VertexBufferContainer::VertexData*)&v1, sizeof(Vertex));
    vb->PushVertex((VertexBufferContainer::VertexData*)&v2, sizeof(Vertex));
    vb->PushVertex((VertexBufferContainer::VertexData*)&v3, sizeof(Vertex));
    
    vb->UpdateVertexBufferIfNeeded();
    
    ib->SetData({0, 1, 2});
    ib->UpdateIndexBufferIfNeeded();
    
    va->AddVertexBuffer(vb);
    va->SetIndexBuffer(ib);
    
    return va;
}

Core::Ref<VertexArrayContainer> DefaultMesh::CreateQuad() {
    auto va = Core::CreateRef<VertexArrayContainer>();
    auto vb = Core::CreateRef<VertexBufferContainer>();
    
    vb->PushLayoutElement({OpenGL::ShaderDataType::Float3, "Pos"});
    
    glm::vec3 v1 = {-1.0f, 1.0f, 0.0f};
    glm::vec3 v2 = {1.0f, 1.0f, 0.0f};
    glm::vec3 v3 = {1.0f, -1.0f, 0.0f};
    glm::vec3 v4 = {-1.0f, -1.0f, 0.0f};
    
    vb->PushVertex((VertexBufferContainer::VertexData*)&v1, sizeof(glm::vec3));
    vb->PushVertex((VertexBufferContainer::VertexData*)&v2, sizeof(glm::vec3));
    vb->PushVertex((VertexBufferContainer::VertexData*)&v3, sizeof(glm::vec3));
    vb->PushVertex((VertexBufferContainer::VertexData*)&v4, sizeof(glm::vec3));
    
    vb->UpdateVertexBufferIfNeeded();
    
    va->GetIndexConteiner()->SetData({0, 1, 2, 2, 0, 3});
    va->GetIndexConteiner()->UpdateIndexBufferIfNeeded();
    
    va->AddVertexBuffer(vb);
    return va;
}

Core::Ref<VertexArrayContainer> DefaultMesh::CreateCube() {
    auto va = Core::CreateRef<VertexArrayContainer>();
    auto vb = Core::CreateRef<VertexBufferContainer>();
    
    vb->PushLayoutElement({OpenGL::ShaderDataType::Float3, "Pos"});
    vb->PushLayoutElement({OpenGL::ShaderDataType::Float3, "Color"});
    
    struct Vertex {
        glm::vec3 pos;
        glm::vec3 color;
    };
    
    float a = 0.5f;
    
    Vertex v0 = {{-a, -a, +a}, {1, 0, 0}};
    Vertex v1 = {{+a, -a, +a}, {0, 1, 0}};
    Vertex v2 = {{+a, +a, +a}, {0, 0, 1}};
    Vertex v3 = {{-a, +a, +a}, {1, 1, 0}};
    
    Vertex v4 = {{+a, -a, -a}, {1, 1, 1}};
    Vertex v5 = {{+a, +a, -a}, {0, 1, 1}};
    Vertex v6 = {{-a, -a, -a}, {1, 0, 1}};
    Vertex v7 = {{-a, +a, -a}, {0, 0, 0}};
    
    vb->PushVertex((VertexBufferContainer::VertexData*)&v0, sizeof(Vertex));
    vb->PushVertex((VertexBufferContainer::VertexData*)&v1, sizeof(Vertex));
    vb->PushVertex((VertexBufferContainer::VertexData*)&v2, sizeof(Vertex));
    vb->PushVertex((VertexBufferContainer::VertexData*)&v3, sizeof(Vertex));
    vb->PushVertex((VertexBufferContainer::VertexData*)&v4, sizeof(Vertex));
    vb->PushVertex((VertexBufferContainer::VertexData*)&v5, sizeof(Vertex));
    vb->PushVertex((VertexBufferContainer::VertexData*)&v6, sizeof(Vertex));
    vb->PushVertex((VertexBufferContainer::VertexData*)&v7, sizeof(Vertex));
    
    vb->UpdateVertexBufferIfNeeded();
    
    va->GetIndexConteiner()->SetData({
        0, 1, 2, 2, 3, 0,
        2, 1, 4, 4, 5, 2,
        0, 3, 7, 7, 6, 0,
        7, 6, 4, 4, 5, 7,
        3, 7, 2, 2, 5, 7,
        0, 1, 4, 4, 6, 0}
    );
    va->GetIndexConteiner()->UpdateIndexBufferIfNeeded();
    
    va->AddVertexBuffer(vb);
    return va;
}


}
