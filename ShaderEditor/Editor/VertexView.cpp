//
//  VertexView.cpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 24.04.2022.
//

#include "Core/pch.h"
#include "VertexView.hpp"
#include "ImGuiHelper.h"

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
            ImGui::TableSetupColumn(element.Name.c_str());
        }
        
        ImGui::TableNextRow(ImGuiTableRowFlags_Headers);
        
        for(int i=0; i<layoutElements.size(); i++) {
            ImGui::TableSetColumnIndex(i);
            ImGui::PushID(i);
            
            ImGui::Text("%s", layoutElements[i].Name.c_str());
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
                
                auto componentType = m_VertexBuffer.GetLayoutElements()[j].Type;
                char* componentData = m_VertexBuffer.GetVertexComponent(i, j);
                ImGui::PushItemWidth(ImGui::GetColumnWidth());
                DrawTableDataInput(componentType, componentData);
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

void VertexView::DrawTableDataInput(const OpenGL::ShaderDataType& type, void* data)
{
    switch (type)
    {
    case OpenGL::ShaderDataType::None:
        ImGui::Text("None");
        break;
    case OpenGL::ShaderDataType::Float:
        ImGui::InputFloat("##float", (float*)data);
        break;
    case OpenGL::ShaderDataType::Float2:
        ImGui::InputFloat2("##float2", (float*)data);
        break;
    case OpenGL::ShaderDataType::Float3:
        ImGui::InputFloat3("##float3", (float*)data);
        break;
    case OpenGL::ShaderDataType::Float4:
        ImGui::InputFloat4("##float4", (float*)data);
        break;
    case OpenGL::ShaderDataType::Mat3: {
        float* dataPtr = (float*)data;
        ImGui::InputFloat3("##mat3_1", dataPtr);
        ImGui::InputFloat3("##mat3_2", dataPtr + 3);
        ImGui::InputFloat3("##mat3_3", dataPtr + 6);
        break;
    }
    case OpenGL::ShaderDataType::Mat4: {
        float* dataPtr = (float*)data;
        ImGui::InputFloat4("##mat4_1", dataPtr);
        ImGui::InputFloat4("##mat4_2", dataPtr + 4);
        ImGui::InputFloat4("##mat4_3", dataPtr + 8);
        ImGui::InputFloat4("##mat4_4", dataPtr + 12);
        break;
    }
    case OpenGL::ShaderDataType::Int:
        ImGui::InputInt("##int", (int*)data);
        break;
    case OpenGL::ShaderDataType::Int2:
        ImGui::InputInt2("##int2", (int*)data);
        break;
    case OpenGL::ShaderDataType::Int3:
        ImGui::InputInt3("##int3", (int*)data);
        break;
    case OpenGL::ShaderDataType::Int4:
        ImGui::InputInt4("##int4", (int*)data);
        break;
    case OpenGL::ShaderDataType::Bool:
        ImGui::Checkbox("##bool", (bool*)data);
        break;
    default:
        ImGui::Text("Error");
        break;
    }
}

void VertexView::DrawBufferElementPopup(int index) {
    if(ImGui::BeginPopup("BufferElementPopup")) {
        
        ImGui::InputText("##elementName", &m_VertexBuffer.GetLayoutElements()[index].Name);
        
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

