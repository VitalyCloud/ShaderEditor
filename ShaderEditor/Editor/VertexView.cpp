//
//  VertexView.cpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 01.05.2022.
//

#include "VertexView.hpp"
#include "ImGuiHelper.hpp"

namespace Editor {

void VertexView::Draw() {
    if(m_VertexBufferContext == nullptr)
        return;
    
    auto layoutElements = m_VertexBufferContext->GetLayoutElements();
    
    if(ImGui::BeginTable("VertexBufferTable", (int)layoutElements.size()+1)) {
        // Setup columns
        ImGui::TableSetupColumn("Index", ImGuiTableColumnFlags_WidthFixed, 50);
        for(auto& element: layoutElements) {
            ImGui::TableSetupColumn(element.Element.Name.c_str());
        }
        ImGui::TableNextRow(ImGuiTableRowFlags_Headers);
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("Index");
        if(ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
            ImGui::OpenPopup("NewVertexComponentFirst");
        }
        if(ImGui::BeginPopup("NewVertexComponentFirst")) {
            for(int i=1; i<OpenGL::ShaderDataTypeCount()-1; i++) {
                OpenGL::ShaderDataType type = static_cast<OpenGL::ShaderDataType>(i);
                if(ImGui::Selectable(OpenGL::ShaderDataTypeTitle(type))) {
                    m_VertexBufferContext->PushLayoutElement(OpenGL::BufferElement(type, "Element"));
                    m_VertexBufferContext->GetState().Set(VertexBufferState::LayoutChanged);
                }
            }
            ImGui::EndPopup();
        }
        
        for(int i=1; i<layoutElements.size()+1; i++) {
            ImGui::TableSetColumnIndex(i);
            ImGui::PushID(i);
            
            ImGui::Text("%s", layoutElements[i-1].Element.Name.c_str());
            if(ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
                ImGui::OpenPopup("BufferElementPopup");
            }
            
            DrawBufferElementPopup(i-1);
            
            ImGui::PopID();
        }
        
        // Draw verticies
        for(int i=0; i<m_VertexBufferContext->Count(); i++) {
            ImGui::PushID(i);
            ImGui::TableNextRow();
            
            // Draw vertex ID
            ImGui::PushID(0);
            ImGui::TableSetColumnIndex(0);
            if(ImGui::Button("x")) {
                m_VertexIndexToDelete = i;
            }
            ImGui::SameLine();
            ImGui::Text("%d", i);
            ImGui::PopID();
            
            // Draw layout elments
            for(int j=1; j<layoutElements.size()+1; j++) {
                ImGui::PushID(j);
                ImGui::TableSetColumnIndex(j);
                
                VertexBufferContainer::VertexData* componentData = m_VertexBufferContext->GetVertexComponent(i, j-1);
                ImGui::PushItemWidth(ImGui::GetColumnWidth());
                
                bool changed = DrawTableDataInput(m_VertexBufferContext->GetLayoutElements()[j-1], componentData);
                
                if(changed)
                    m_VertexBufferContext->GetState().Set(VertexBufferState::DataChanged);
                
            
                ImGui::PopItemWidth();
                
                ImGui::PopID();
            }
            ImGui::PopID();
        }
        ImGui::EndTable();
    }
    
    if(ImGui::Button("+##AddVertex")) {
        m_VertexBufferContext->PushVertex();
        m_VertexBufferContext->GetState().Set(VertexBufferState::SizeChanged);
    }
    
    if(ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
        ImGui::OpenPopup("NewVertexComponent");
    }
    
    if(ImGui::BeginPopup("NewVertexComponent")) {
        for(int i=1; i<OpenGL::ShaderDataTypeCount()-1; i++) {
            OpenGL::ShaderDataType type = static_cast<OpenGL::ShaderDataType>(i);
            if(ImGui::Selectable(OpenGL::ShaderDataTypeTitle(type))) {
                m_VertexBufferContext->PushLayoutElement(OpenGL::BufferElement(type, "Element"));
                m_VertexBufferContext->GetState().Set(VertexBufferState::LayoutChanged);
            }
        }
        ImGui::EndPopup();
    }
    
    if(m_VertexIndexToDelete > -1) {
        m_VertexBufferContext->PopVertex(m_VertexIndexToDelete);
        m_VertexBufferContext->GetState().Set(VertexBufferState::SizeChanged);
    }
        
    m_VertexIndexToDelete = -1;
    if(m_VertexLayoutElementToDelete > -1) {
        m_VertexBufferContext->PopLayoutElement(m_VertexLayoutElementToDelete);
        m_VertexBufferContext->GetState().Set(VertexBufferState::LayoutChanged);
    }
       
    m_VertexLayoutElementToDelete = -1;
        
}

void VertexView::DrawBufferElementPopup(int index) {
    if(ImGui::BeginPopup("BufferElementPopup")) {
        
        ImGui::InputText("##elementName", &m_VertexBufferContext->GetLayoutElements()[index].Element.Name);
        
        if(ImGui::Selectable("Delete")) {
            m_VertexLayoutElementToDelete = index;
        }
        if(ImGui::BeginMenu("Change Type")) {
            for(int i=1; i<OpenGL::ShaderDataTypeCount(); i++) {
                OpenGL::ShaderDataType type = static_cast<OpenGL::ShaderDataType>(i);
                if(ImGui::Selectable(OpenGL::ShaderDataTypeTitle(type))) {
                    m_VertexBufferContext->ChangeLayoutElementType(index, type);
                    m_VertexBufferContext->GetState().Set(VertexBufferState::LayoutChanged);
                }
            }
            ImGui::EndMenu();
        }
        if(ImGui::BeginMenu("View")) {
            ImGui::DrawInputSettingsView(m_VertexBufferContext->GetLayoutElements()[index].Element.Type,
                                  &m_VertexBufferContext->GetLayoutElements()[index].Settings);
            ImGui::EndMenu();
        }
        
        ImGui::Separator();
        if(ImGui::BeginMenu("Insert Left")) {
            for(int i=1; i<OpenGL::ShaderDataTypeCount(); i++) {
                OpenGL::ShaderDataType type = static_cast<OpenGL::ShaderDataType>(i);
                if(ImGui::Selectable(OpenGL::ShaderDataTypeTitle(type))) {
                    OpenGL::BufferElement element = OpenGL::BufferElement(type, "Element");
                    m_VertexBufferContext->InsertLayoutElement(index, element);
                    m_VertexBufferContext->GetState().Set(VertexBufferState::LayoutChanged);
                }
            }
            ImGui::EndMenu();
        }
        if(ImGui::BeginMenu("Insert Right")) {
            for(int i=1; i<OpenGL::ShaderDataTypeCount(); i++) {
                OpenGL::ShaderDataType type = static_cast<OpenGL::ShaderDataType>(i);
                if(ImGui::Selectable(OpenGL::ShaderDataTypeTitle(type))) {
                    OpenGL::BufferElement element = OpenGL::BufferElement(type, "Element");
                    m_VertexBufferContext->InsertLayoutElement(index+1, element);
                    m_VertexBufferContext->GetState().Set(VertexBufferState::LayoutChanged);
                }
            }
            ImGui::EndMenu();
        }
        ImGui::EndPopup();
    }
}

bool VertexView::DrawTableDataInput(const VertexBufferElement& element, void* data) {
    bool changed = false;
    switch (element.Element.Type)
    {
        case OpenGL::ShaderDataType::Float:
        case OpenGL::ShaderDataType::Float2:
        case OpenGL::ShaderDataType::Float3:
        case OpenGL::ShaderDataType::Float4:
            if(ImGui::DrawInputFloat((float*)data, &element.Settings, OpenGL::ShaderDataTypeComponentCount(element.Element.Type))) {
                changed = true;
            }
            break;
        case OpenGL::ShaderDataType::Int:
        case OpenGL::ShaderDataType::Int2:
        case OpenGL::ShaderDataType::Int3:
        case OpenGL::ShaderDataType::Int4:
            if(ImGui::DrawInputInt((int*)data, &element.Settings, OpenGL::ShaderDataTypeComponentCount(element.Element.Type))) {
                changed = true;
            }
            break;
        case OpenGL::ShaderDataType::Bool:
            if(ImGui::Checkbox("##bool", (bool*)data)) {
                changed = true;
            }
            break;
        
        case OpenGL::ShaderDataType::Mat3: {
            if(ImGui::DrawInputMat((float*)data, &element.Settings, 3)) {
                changed = true;
            }
            break;
        }
        case OpenGL::ShaderDataType::Mat4: {
            if(ImGui::DrawInputMat((float*)data, &element.Settings, 4)) {
                changed = true;
            }
            break;
        }
        default:
            return false;
    }
    return changed;
}



}
