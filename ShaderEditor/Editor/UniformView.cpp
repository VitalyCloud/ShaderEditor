//
//  UniformView.cpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 23.04.2022.
//
#include "Core/pch.h"
#include "UniformView.hpp"
#include "Core/Core.hpp"
#include "ImGuiHelper.h"

namespace Editor {

UniformView::UniformView() {
    m_UniformsToDelete.reserve(5);
}

UniformView::~UniformView() {
    
}

static const char* const s_Types[] = {
    "None",
    "Float",
    "Float2",
    "Float3",
    "Float4",
    "Mat3",
    "Mat4",
    "Int",
    "Int2",
    "Int3",
    "Int4",
    "Bool"
};

void UniformView::Draw()  {
    ImGui::BeginChild("UniformView");
    
    if(ImGui::BeginTable("Uniforms", 3, ImGuiTableFlags_Resizable)) {
        ImGui::TableSetupColumn("Titile");
        ImGui::TableSetupColumn("Type");
        ImGui::TableSetupColumn("Value");
        ImGui::TableHeadersRow();
        
        for(int i=0; i<m_Uniforms.Size(); i++) {
            ImGui::TableNextRow();
            ImGui::PushID(i);
            
            // Title
            ImGui::TableSetColumnIndex(0);
            ImGui::PushItemWidth(ImGui::GetColumnWidth());
            if(ImGui::Button("x")) {
                m_UniformsToDelete.push_back(i);
            }
            ImGui::SameLine();
            ImGui::InputText("##UniformName", &m_Uniforms.GetUniform(i).Title);
            ImGui::PopItemWidth();
            
            auto uniform = m_Uniforms.GetUniform(i);
            // Type
            ImGui::TableSetColumnIndex(1);
            ImGui::PushItemWidth(ImGui::GetColumnWidth());
            int selectedType = static_cast<int>(uniform.Type) - 1;
            if(ImGui::Combo("##Type", &selectedType, s_Types+1, OpenGL::ShaderDataTypeCount() -1)) {
                auto newType = OpenGL::ShaderDataType(selectedType + 1);
                m_Uniforms.ChangeType(i, newType);
            }
            ImGui::PopItemWidth();
            
            // Value
            ImGui::TableSetColumnIndex(2);
            ImGui::PushItemWidth(ImGui::GetColumnWidth()-20);
            DrawUniformInput(uniform, m_Uniforms.GetUniformData(i));
            ImGui::PopItemWidth();
            if(uniform.Type != OpenGL::ShaderDataType::Bool) {
                ImGui::SameLine();
                if(ImGui::Button(":"))
                    ImGui::OpenPopup("InputSettings");
            }
            DrawInputSettings(i);
            
            ImGui::PopID();
        }
        ImGui::EndTable();
    }
    if(ImGui::Button("Add unifom")) {
        m_Uniforms.PushUnifrom();
    }
    if(ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
        ImGui::OpenPopup("NewUniformType");
    }
    if(ImGui::BeginPopup("NewUniformType")) {
        for(int i=1; i<OpenGL::ShaderDataTypeCount(); i++) {
            OpenGL::ShaderDataType type = static_cast<OpenGL::ShaderDataType>(i);
            if(ImGui::Selectable(OpenGL::ShaderDataTypeTitle(type))) {
                m_Uniforms.PushUniform(type);
            }
        }
        ImGui::EndPopup();
    }
    
    // Update Data
    for(int i=0; i<m_UniformsToDelete.size(); i++)
        m_Uniforms.Delete(m_UniformsToDelete[i]);
    m_UniformsToDelete.clear();
    
    ImGui::EndChild();
}

static const char* const s_WidgetType[] = {
    "Input",
    "Drag",
    "Slider",
    "Color",
};

void UniformView::DrawInputSettings(int i) {
    if(ImGui::BeginPopup("InputSettings")) {
        int count = 3;
        if(m_Uniforms.GetUniform(i).Type == OpenGL::ShaderDataType::Float3 ||
           m_Uniforms.GetUniform(i).Type == OpenGL::ShaderDataType::Float4)
            count = 4;
        
        int current = static_cast<int>(m_Uniforms.GetUniform(i).Settings.Type);
        ImGui::PushItemWidth(200);
        if(ImGui::ListBox("##items", &current, s_WidgetType, count)) {
            EN_INFO("Selected: {0}", s_WidgetType[current]);
            m_Uniforms.GetUniform(i).Settings.Type = ImGuiWidgetType(current);
        }
        ImGui::PopItemWidth();
        
        if(m_Uniforms.GetUniform(i).Settings.Type == ImGuiWidgetType::Slider ||
           (m_Uniforms.GetUniform(i).Type != OpenGL::ShaderDataType::Int &&
           m_Uniforms.GetUniform(i).Type != OpenGL::ShaderDataType::Int2 &&
           m_Uniforms.GetUniform(i).Type != OpenGL::ShaderDataType::Int3 &&
           m_Uniforms.GetUniform(i).Type != OpenGL::ShaderDataType::Int4)) {
            
            if(m_Uniforms.GetUniform(i).Settings.Type == ImGuiWidgetType::Drag) {
                ImGui::Separator();
                ImGui::Checkbox("UseRange", &m_Uniforms.GetUniform(i).Settings.UseRange);
                if(m_Uniforms.GetUniform(i).Settings.UseRange) {
                    ImGui::PushItemWidth(100);
                    ImGui::InputFloat("Min", &m_Uniforms.GetUniform(i).Settings.Min);
                    ImGui::InputFloat("Max", &m_Uniforms.GetUniform(i).Settings.Max);
                    ImGui::PopItemWidth();
                }
            } else if (m_Uniforms.GetUniform(i).Settings.Type == ImGuiWidgetType::Slider) {
                ImGui::Separator();
                ImGui::PushItemWidth(100);
                ImGui::InputFloat("Min", &m_Uniforms.GetUniform(i).Settings.Min);
                ImGui::InputFloat("Max", &m_Uniforms.GetUniform(i).Settings.Max);
                ImGui::PopItemWidth();
            }
        }
        ImGui::EndPopup();
    }
}

void UniformView::DrawUniformInput(Uniform& uniform, void* data) {
    switch (uniform.Type) {
        case OpenGL::ShaderDataType::Float:
        case OpenGL::ShaderDataType::Float2:
        case OpenGL::ShaderDataType::Float3:
        case OpenGL::ShaderDataType::Float4:
            ImGui::DrawInputFloat((float*)data, &uniform.Settings, OpenGL::ShaderDataTypeComponentCount(uniform.Type));
            break;
        case OpenGL::ShaderDataType::Int:
        case OpenGL::ShaderDataType::Int2:
        case OpenGL::ShaderDataType::Int3:
        case OpenGL::ShaderDataType::Int4:
            ImGui::DrawInputInt((int*)data, &uniform.Settings, OpenGL::ShaderDataTypeComponentCount(uniform.Type));
            break;
        case OpenGL::ShaderDataType::Bool:
            ImGui::Checkbox("##bool", (bool*)data);
            break;
        
        case OpenGL::ShaderDataType::Mat3: {
            ImGui::DrawInputMat((float*)data, &uniform.Settings, 3);
            break;
        }
        case OpenGL::ShaderDataType::Mat4: {
            ImGui::DrawInputMat((float*)data, &uniform.Settings, 4);
            break;
        }
        default:
            return;
    }
}

}
