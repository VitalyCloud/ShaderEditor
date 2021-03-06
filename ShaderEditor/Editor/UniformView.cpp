//
//  UniformView.cpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 01.05.2022.
//
#include "pch.h"
#include "UniformView.hpp"

namespace Editor {

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

void UniformView::Draw() {
    if(m_Uniforms == nullptr)
        return;
    
    if(ImGui::BeginTable("Uniforms", 3, ImGuiTableFlags_Resizable)) {
        ImGui::TableSetupColumn("Title");
        ImGui::TableSetupColumn("Type");
        ImGui::TableSetupColumn("Value");
        ImGui::TableHeadersRow();
        
        for(int i=0; i<m_Uniforms->Count(); i++) {
            ImGui::TableNextRow();
            ImGui::PushID(i);
            
            // Title
            ImGui::TableSetColumnIndex(0);
            ImGui::PushItemWidth(ImGui::GetColumnWidth());
            if(ImGui::Button("x")) {
                m_UniformToDelete = i;
            }
            ImGui::SameLine();
            ImGui::InputText("##UniformName", &m_Uniforms->GetUniform(i).Title);
            ImGui::PopItemWidth();
            
            auto uniform = m_Uniforms->GetUniform(i);
            // Color
            ImU32 cell_bg_color = ImGui::GetColorU32(ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
            if(uniform.Connected)
                cell_bg_color = ImGui::GetColorU32(ImVec4(0.084f, 0.510f, 0.094f, 0.35f));
            ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0, cell_bg_color);
            
            // Type
            ImGui::TableSetColumnIndex(1);
            ImGui::PushItemWidth(ImGui::GetColumnWidth());
            int selectedType = static_cast<int>(uniform.Type) - 1;
            if(ImGui::Combo("##Type", &selectedType, s_Types+1, OpenGL::ShaderDataTypeCount() - 2)) {
                auto newType = OpenGL::ShaderDataType(selectedType + 1);
                m_Uniforms->ChangeUniformType(i, newType);
            }
            ImGui::PopItemWidth();
            
            // Value
            ImGui::TableSetColumnIndex(2);
            ImGui::PushItemWidth(ImGui::GetColumnWidth()-20);
            DrawUniformInput(uniform, m_Uniforms->GetUniformData(i));
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
    if(ImGui::Button("Add uniform")) {
        m_Uniforms->PushUniform();
    }
    if(ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
        ImGui::OpenPopup("NewUniformType");
    }
    if(ImGui::BeginPopup("NewUniformType")) {
        for(int i=1; i<OpenGL::ShaderDataTypeCount(); i++) {
            OpenGL::ShaderDataType type = static_cast<OpenGL::ShaderDataType>(i);
            if(ImGui::Selectable(OpenGL::ShaderDataTypeTitle(type))) {
                m_Uniforms->PushUniform(type);
            }
        }
        ImGui::EndPopup();
    }
    
    if(m_UniformToDelete > -1)
        m_Uniforms->PopUniform(m_UniformToDelete);
    m_UniformToDelete = -1;
}


void UniformView::DrawInputSettings(int index) {
    Uniform& uniform = (*m_Uniforms)[index];
    OpenGL::ShaderDataType type = uniform.Type;
    ImGuiInputSettings* settings = &uniform.Settings;

    if(ImGui::BeginPopup("InputSettings")) {
        ImGui::DrawInputSettingsView(type, settings);
        ImGui::EndPopup();
    }
}

void UniformView::DrawUniformInput(Uniform& uniform, UniformBufferContainer::UniformData* data) {
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
        case OpenGL::ShaderDataType::Mat3: {
            ImGui::DrawInputMat((float*)data, &uniform.Settings, 3);
            break;
        }
        case OpenGL::ShaderDataType::Mat4: {
            ImGui::DrawInputMat((float*)data, &uniform.Settings, 4);
            break;
        }
        case OpenGL::ShaderDataType::Bool:
            break;
        default:
            return;
    }
}

}
