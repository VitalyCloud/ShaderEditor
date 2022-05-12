//
//  Mesh.cpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 01.05.2022.
//
#include "pch.h"
#include "Mesh.hpp"
#include "ImGuiHelper.hpp"
#include "PropertyTable.hpp"
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
        
        // Texture binding
        for(int i=0; i<m_Textures.size(); i++) {
            auto& texture = m_Textures[i];
            texture->Bind(i);
            if(!texture->GetTitle().empty()) {
                shader->SetInt(texture->GetTitle(), i);
            }
        }
        
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

static const char* s_Primitives[] = {
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

    ImGui::Text("Mesh"); ImGui::Separator();
    if(PropertyTable::Begin("MeshPropertiesTable")) {
        
        PropertyTable::Text("Title", m_Context->m_Title);
        
        PropertyTable::Checkbox("Use Index Buffer", m_Context->m_UseIndexBuffer);
        
        bool positionAction = false;
        ImGui::PushID("Pos");
        PropertyTable::InputFloat("Position", &m_Context->m_Transform.Translation.x, &m_PositionSettings, 3, &positionAction);
        if(positionAction)
            ImGui::OpenPopup("MeshPositionInputSettings");
        if(ImGui::BeginPopup("MeshPositionInputSettings")) {
            ImGui::DrawInputSettingsView(OpenGL::ShaderDataType::Float3, &m_PositionSettings);
            ImGui::EndPopup();
        }
        ImGui::PopID();
        
        ImGui::PushID("Rot");
        bool rotationAction = false;
        PropertyTable::InputFloat("Rotation", &m_Context->m_Transform.Rotation.x, &m_RrotationSettings, 3, &rotationAction);
        if(rotationAction)
            ImGui::OpenPopup("MeshRotationInputSettings");
        if(ImGui::BeginPopup("MeshRotationInputSettings")) {
            ImGui::DrawInputSettingsView(OpenGL::ShaderDataType::Float3, &m_RrotationSettings);
            ImGui::EndPopup();
        }
        ImGui::PopID();
        
        ImGui::PushID("Scl");
        bool scaleAction = false;
        PropertyTable::InputFloat("Scale", &m_Context->m_Transform.Scale.x, &m_ScaleSettings, 3, &scaleAction);
        if(scaleAction)
            ImGui::OpenPopup("MeshScaleInputSettings");
        if(ImGui::BeginPopup("MeshScaleInputSettings")) {
            ImGui::DrawInputSettingsView(OpenGL::ShaderDataType::Float3, &m_ScaleSettings);
            ImGui::EndPopup();
        }
        ImGui::PopID();
        
        PropertyTable::Combo("Topology", s_Primitives, IM_ARRAYSIZE(s_Primitives), m_Context->m_Topology);
        
        PropertyTable::End();
    }
    
    m_VertexArrayView.SetContext(m_Context->m_VertexArrayContainer);
    m_VertexArrayView.Draw();
    
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
