//
//  VertexView.cpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 24.04.2022.
//

#include "Core/pch.h"
#include "VertexView.hpp"
#include "ImGuiHelper.h"
#include "ImGuiInputSettings.hpp"

#include "imgui.h"

namespace Editor {

VertexView::VertexView() {
    
}

VertexView::~VertexView() {
    
}

void VertexView::Draw() {
    ImGui::BeginChild("VertexBufferTableView");
    auto layoutElements = m_VertexBuffer.GetLayoutElements();
    if(ImGui::BeginTable("VertexBufferTable", (int)layoutElements.size())) {
        
        for(auto& element: layoutElements) {
            ImGui::TableSetupColumn(element.Element.Name.c_str());
        }
        
        ImGui::TableNextRow(ImGuiTableRowFlags_Headers);
        
        for(int i=0; i<layoutElements.size(); i++) {
            ImGui::TableSetColumnIndex(i);
            ImGui::PushID(i);
            
            ImGui::Text("%s", layoutElements[i].Element.Name.c_str());
            if(ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
                ImGui::OpenPopup("BufferElementPopup");
            }
            
            DrawBufferElementPopup(i);
            
            ImGui::PopID();
        }
        
        for(int i=0; i<m_VertexBuffer.VertexCount(); i++) {
            ImGui::PushID(i);
            ImGui::TableNextRow();
            
            
            for(int j=0; j<layoutElements.size(); j++) {
                ImGui::PushID(j);
                ImGui::TableSetColumnIndex(j);
                
                char* componentData = m_VertexBuffer.GetVertexComponent(i, j);
                ImGui::PushItemWidth(ImGui::GetColumnWidth());
                DrawTableDataInput(m_VertexBuffer.GetLayoutElements()[j], componentData);
                ImGui::PopItemWidth();
                
                ImGui::PopID();
            }
            
            
            ImGui::PopID();
        }
        
        
        ImGui::EndTable();
    }
    
    if(ImGui::Button("Add buffer")) {
        m_VertexBuffer.PushVertex();
    }
    
    if(ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
        ImGui::OpenPopup("NewVertexComponent");
    }
    
    if(ImGui::BeginPopup("NewVertexComponent")) {
        for(int i=1; i<OpenGL::ShaderDataTypeCount(); i++) {
            OpenGL::ShaderDataType type = static_cast<OpenGL::ShaderDataType>(i);
            if(ImGui::Selectable(OpenGL::ShaderDataTypeTitle(type))) {
                OpenGL::BufferElement element = OpenGL::BufferElement(type, "Test");
                m_VertexBuffer.PushLayoutElement(element);
            }
        }
        ImGui::EndPopup();
    }
    ImGui::EndChild();
}

void VertexView::DrawTableDataInput(const VertexBufferElement& element, void* data)
{
    switch (element.Element.Type)
    {
        case OpenGL::ShaderDataType::Float:
        case OpenGL::ShaderDataType::Float2:
        case OpenGL::ShaderDataType::Float3:
        case OpenGL::ShaderDataType::Float4:
            DrawInputFloat((float*)data, &element.Settings, OpenGL::ShaderDataTypeComponentCount(element.Element.Type));
            break;
        case OpenGL::ShaderDataType::Int:
        case OpenGL::ShaderDataType::Int2:
        case OpenGL::ShaderDataType::Int3:
        case OpenGL::ShaderDataType::Int4:
            DrawInputInt((int*)data, &element.Settings, OpenGL::ShaderDataTypeComponentCount(element.Element.Type));
            break;
        case OpenGL::ShaderDataType::Bool:
            ImGui::Checkbox("##bool", (bool*)data);
            break;
        
        case OpenGL::ShaderDataType::Mat3: {
            DrawInputMat((float*)data, &element.Settings, 3);
            break;
        }
        case OpenGL::ShaderDataType::Mat4: {
            DrawInputMat((float*)data, &element.Settings, 4);
            break;
        }
        default:
            return;
    }
}

void VertexView::DrawBufferElementPopup(int index) {
    if(ImGui::BeginPopup("BufferElementPopup")) {
        
        ImGui::InputText("##elementName", &m_VertexBuffer.GetLayoutElements()[index].Element.Name);
        
        if(ImGui::Selectable("Delete")) {
            if(m_VertexBuffer.GetLayoutElements().size() > 1)
                m_VertexBuffer.RemoveLayoutElement(index);
        }
        if(ImGui::BeginMenu("Change Type")) {
            for(int i=1; i<OpenGL::ShaderDataTypeCount(); i++) {
                OpenGL::ShaderDataType type = static_cast<OpenGL::ShaderDataType>(i);
                if(ImGui::Selectable(OpenGL::ShaderDataTypeTitle(type))) {
                    m_VertexBuffer.ChangeElementType(index, type);
                }
            }
            ImGui::EndMenu();
        }
        if(ImGui::BeginMenu("View")) {
            DrawInputSettingsView(m_VertexBuffer.GetLayoutElements()[index].Element.Type,
                                  &m_VertexBuffer.GetLayoutElements()[index].Settings);
            
            ImGui::EndMenu();
        }
        
        ImGui::Separator();
        if(ImGui::BeginMenu("Insert Left")) {
            for(int i=1; i<OpenGL::ShaderDataTypeCount(); i++) {
                OpenGL::ShaderDataType type = static_cast<OpenGL::ShaderDataType>(i);
                if(ImGui::Selectable(OpenGL::ShaderDataTypeTitle(type))) {
                    OpenGL::BufferElement element = OpenGL::BufferElement(type, "New Element");
                    m_VertexBuffer.InsertLayoutElement(index, element);
                }
            }
            ImGui::EndMenu();
        }
        if(ImGui::BeginMenu("Insert Right")) {
            for(int i=1; i<OpenGL::ShaderDataTypeCount(); i++) {
                OpenGL::ShaderDataType type = static_cast<OpenGL::ShaderDataType>(i);
                if(ImGui::Selectable(OpenGL::ShaderDataTypeTitle(type))) {
                    OpenGL::BufferElement element = OpenGL::BufferElement(type, "New Element");
                    m_VertexBuffer.InsertLayoutElement(index+1, element);
                }
            }
            ImGui::EndMenu();
        }
        ImGui::EndPopup();
    }
}

}

