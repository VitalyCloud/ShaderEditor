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
    if(m_VertexBufferContext == nullptr)
        return;
//    ImVec2 size = ImVec2(0, 260);
//    ImGui::BeginChild("VertexBufferTableView", size);
    auto layoutElements = m_VertexBufferContext->GetLayoutElements();
    if(ImGui::BeginTable("VertexBufferTable", (int)layoutElements.size()+1)) {
        
        ImGui::TableSetupColumn("Index", ImGuiTableColumnFlags_WidthFixed, 50);
        for(auto& element: layoutElements) {
            ImGui::TableSetupColumn(element.Element.Name.c_str());
        }
        
        ImGui::TableNextRow(ImGuiTableRowFlags_Headers);
        
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("Index");
        
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
        
        for(int i=0; i<m_VertexBufferContext->VertexCount(); i++) {
            ImGui::PushID(i);
            ImGui::TableNextRow();
            
            ImGui::PushID(0);
            ImGui::TableSetColumnIndex(0);
            if(ImGui::Button("x")) {
                EN_INFO("Delete {0}", i);
                m_VertexBufferContext->RemoveVertex(i);
            }
            ImGui::SameLine();
            ImGui::Text("%d", i);
            ImGui::PopID();
            
            for(int j=1; j<layoutElements.size()+1; j++) {
                ImGui::PushID(j);
                ImGui::TableSetColumnIndex(j);
                
                char* componentData = m_VertexBufferContext->GetVertexComponent(i, j-1);
                ImGui::PushItemWidth(ImGui::GetColumnWidth());
                bool changed = DrawTableDataInput(m_VertexBufferContext->GetLayoutElements()[j-1], componentData);
                
                if(changed && m_AutoChange)
                    m_VertexBufferContext->SetChanged(changed);
                ImGui::PopItemWidth();
                
                ImGui::PopID();
            }
            
            
            ImGui::PopID();
        }
        
        
        ImGui::EndTable();
    }
    
    if(ImGui::Button("+##AddVertex")) {
        m_VertexBufferContext->PushVertex();
        if(m_AutoChange) m_VertexBufferContext->SetChanged(true);
    }
    
    if(ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
        ImGui::OpenPopup("NewVertexComponent");
    }
    
    ImGui::SameLine();
    ImGui::Checkbox("Auto change", &m_AutoChange);
    if(!m_AutoChange) {
        ImGui::SameLine();
        if(ImGui::Button("Apply"))
            m_VertexBufferContext->SetChanged(true);
    }
    
    if(ImGui::BeginPopup("NewVertexComponent")) {
        for(int i=1; i<OpenGL::ShaderDataTypeCount(); i++) {
            OpenGL::ShaderDataType type = static_cast<OpenGL::ShaderDataType>(i);
            if(ImGui::Selectable(OpenGL::ShaderDataTypeTitle(type))) {
                OpenGL::BufferElement element = OpenGL::BufferElement(type, "Test");
                m_VertexBufferContext->PushLayoutElement(element);
                if(m_AutoChange) m_VertexBufferContext->SetChanged(true);
            }
        }
        ImGui::EndPopup();
    }
//    ImGui::EndChild();
}

bool VertexView::DrawTableDataInput(const VertexBufferElement& element, void* data)
{
    bool changed = false;
    switch (element.Element.Type)
    {
        case OpenGL::ShaderDataType::Float:
        case OpenGL::ShaderDataType::Float2:
        case OpenGL::ShaderDataType::Float3:
        case OpenGL::ShaderDataType::Float4:
            if(DrawInputFloat((float*)data, &element.Settings, OpenGL::ShaderDataTypeComponentCount(element.Element.Type))) {
                changed = true;
            }
            break;
        case OpenGL::ShaderDataType::Int:
        case OpenGL::ShaderDataType::Int2:
        case OpenGL::ShaderDataType::Int3:
        case OpenGL::ShaderDataType::Int4:
            if(DrawInputInt((int*)data, &element.Settings, OpenGL::ShaderDataTypeComponentCount(element.Element.Type))) {
                changed = true;
            }
            break;
        case OpenGL::ShaderDataType::Bool:
            if(ImGui::Checkbox("##bool", (bool*)data)) {
                changed = true;
            }
            break;
        
        case OpenGL::ShaderDataType::Mat3: {
            if(DrawInputMat((float*)data, &element.Settings, 3)) {
                changed = true;
            }
            break;
        }
        case OpenGL::ShaderDataType::Mat4: {
            if(DrawInputMat((float*)data, &element.Settings, 4)) {
                changed = true;
            }
            break;
        }
        default:
            return false;
    }
    return changed;
}

void VertexView::DrawBufferElementPopup(int index) {
    if(ImGui::BeginPopup("BufferElementPopup")) {
        
        ImGui::InputText("##elementName", &m_VertexBufferContext->GetLayoutElements()[index].Element.Name);
        
        if(ImGui::Selectable("Delete")) {
            if(m_VertexBufferContext->GetLayoutElements().size() > 1) {
                m_VertexBufferContext->RemoveLayoutElement(index);
                if(m_AutoChange) m_VertexBufferContext->SetChanged(true);
            }
                
        }
        if(ImGui::BeginMenu("Change Type")) {
            for(int i=1; i<OpenGL::ShaderDataTypeCount(); i++) {
                OpenGL::ShaderDataType type = static_cast<OpenGL::ShaderDataType>(i);
                if(ImGui::Selectable(OpenGL::ShaderDataTypeTitle(type))) {
                    m_VertexBufferContext->ChangeElementType(index, type);
                    if(m_AutoChange) m_VertexBufferContext->SetChanged(true);
                }
            }
            ImGui::EndMenu();
        }
        if(ImGui::BeginMenu("View")) {
            DrawInputSettingsView(m_VertexBufferContext->GetLayoutElements()[index].Element.Type,
                                  &m_VertexBufferContext->GetLayoutElements()[index].Settings);
            
            ImGui::EndMenu();
        }
        
        ImGui::Separator();
        if(ImGui::BeginMenu("Insert Left")) {
            for(int i=1; i<OpenGL::ShaderDataTypeCount(); i++) {
                OpenGL::ShaderDataType type = static_cast<OpenGL::ShaderDataType>(i);
                if(ImGui::Selectable(OpenGL::ShaderDataTypeTitle(type))) {
                    OpenGL::BufferElement element = OpenGL::BufferElement(type, "New Element");
                    m_VertexBufferContext->InsertLayoutElement(index, element);
                    if(m_AutoChange) m_VertexBufferContext->SetChanged(true);
                }
            }
            ImGui::EndMenu();
        }
        if(ImGui::BeginMenu("Insert Right")) {
            for(int i=1; i<OpenGL::ShaderDataTypeCount(); i++) {
                OpenGL::ShaderDataType type = static_cast<OpenGL::ShaderDataType>(i);
                if(ImGui::Selectable(OpenGL::ShaderDataTypeTitle(type))) {
                    OpenGL::BufferElement element = OpenGL::BufferElement(type, "New Element");
                    m_VertexBufferContext->InsertLayoutElement(index+1, element);
                    if(m_AutoChange) m_VertexBufferContext->SetChanged(true);
                }
            }
            ImGui::EndMenu();
        }
        ImGui::EndPopup();
    }
}

}

