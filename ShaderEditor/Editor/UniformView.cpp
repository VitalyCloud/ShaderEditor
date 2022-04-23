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
    ImGui::Separator();
    
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
    if(ImGui::Button("+")) {
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
            DrawInputFloat(uniform, data);
            break;
        case OpenGL::ShaderDataType::Int:
        case OpenGL::ShaderDataType::Int2:
        case OpenGL::ShaderDataType::Int3:
        case OpenGL::ShaderDataType::Int4:
            DrawInputInt(uniform, data);
            break;
        case OpenGL::ShaderDataType::Bool:
            ImGui::Checkbox("##bool", (bool*)data);
            break;
        
        case OpenGL::ShaderDataType::Mat3: {
            float* dataPtr = (float*)data;
            ImGui::PushID(0);
            DrawInputFloat(uniform, dataPtr);
            ImGui::PopID();
            ImGui::PushID(1);
            DrawInputFloat(uniform, dataPtr + 3);
            ImGui::PopID();
            ImGui::PushID(2);
            DrawInputFloat(uniform, dataPtr + 6);
            ImGui::PopID();
            break;
        }
        case OpenGL::ShaderDataType::Mat4: {
            float* dataPtr = (float*)data;
            ImGui::PushID(0);
            DrawInputFloat(uniform, dataPtr);
            ImGui::PopID();
            ImGui::PushID(1);
            DrawInputFloat(uniform, dataPtr + 4);
            ImGui::PopID();
            ImGui::PushID(2);
            DrawInputFloat(uniform, dataPtr + 8);
            ImGui::PopID();
            ImGui::PushID(3);
            DrawInputFloat(uniform, dataPtr + 12);
            ImGui::PopID();
            break;
        }
        default:
            return;
    }
}



void UniformView::DrawInputFloat(Uniform& uniform, void* data) {
    switch (uniform.Type) {
        case OpenGL::ShaderDataType::Float:
            switch (uniform.Settings.Type) {
                case ImGuiWidgetType::Input:
                    ImGui::InputFloat("##InputFloat", (float*) data);
                    break;
                case ImGuiWidgetType::Drag:
                    if (uniform.Settings.UseRange)
                        ImGui::DragFloat("##DragFloat", (float*) data, uniform.Settings.Speed, uniform.Settings.Min, uniform.Settings.Max);
                    else
                        ImGui::DragFloat("##DragFloat", (float*) data, uniform.Settings.Speed);
                    break;
                case ImGuiWidgetType::Slider:
                    ImGui::SliderFloat("##SliderFloat", (float*) data, uniform.Settings.Min, uniform.Settings.Max);
                    break;
                case ImGuiWidgetType::Color:
                    break;
            }
            break;
        case OpenGL::ShaderDataType::Float2:
            switch (uniform.Settings.Type) {
                case ImGuiWidgetType::Input:
                    ImGui::InputFloat2("##InputFloat2", (float*) data);
                    break;
                case ImGuiWidgetType::Drag:
                    if (uniform.Settings.UseRange)
                        ImGui::DragFloat2("##DragFloat2", (float*) data, uniform.Settings.Speed, uniform.Settings.Min, uniform.Settings.Max);
                    else
                        ImGui::DragFloat2("##DragFloat2", (float*) data, uniform.Settings.Speed);
                    break;
                case ImGuiWidgetType::Slider:
                    ImGui::SliderFloat2("##SliderFloat2", (float*) data, uniform.Settings.Min, uniform.Settings.Max);
                    break;
                case ImGuiWidgetType::Color:
                    break;
            }
            break;
        case OpenGL::ShaderDataType::Float3:
            switch (uniform.Settings.Type) {
                case ImGuiWidgetType::Input:
                    ImGui::InputFloat3("##InputFloat3", (float*) data);
                    break;
                case ImGuiWidgetType::Drag:
                    if (uniform.Settings.UseRange)
                        ImGui::DragFloat3("##DragFloat3", (float*) data, uniform.Settings.Speed, uniform.Settings.Min, uniform.Settings.Max);
                    else
                        ImGui::DragFloat3("##DragFloat3", (float*) data, uniform.Settings.Speed);
                    break;
                case ImGuiWidgetType::Slider:
                    ImGui::SliderFloat3("##SliderFloat3", (float*) data, uniform.Settings.Min, uniform.Settings.Max);
                    break;
                case ImGuiWidgetType::Color:
                    ImGui::ColorEdit3("##ColorFloat3", (float*) data);
                    break;
            }
            break;
        case OpenGL::ShaderDataType::Float4:
            switch (uniform.Settings.Type) {
                case ImGuiWidgetType::Input: {
                    ImGui::InputFloat4("##InputFloat4", (float*) data);
                    break;
                }
                case ImGuiWidgetType::Drag:
                    if (uniform.Settings.UseRange)
                        ImGui::DragFloat4("##DragFloat4", (float*) data, uniform.Settings.Speed, uniform.Settings.Min, uniform.Settings.Max);
                    else
                        ImGui::DragFloat4("##DragFloat4", (float*) data, uniform.Settings.Speed);
                    break;
                case ImGuiWidgetType::Slider:
                    ImGui::SliderFloat4("##SliderFloat4", (float*) data, uniform.Settings.Min, uniform.Settings.Max);
                    break;
                case ImGuiWidgetType::Color:
                    ImGui::ColorEdit4("##ColorFloat4", (float*) data, ImGuiColorEditFlags_NoTooltip);
                    break;
            }
            break;
        case OpenGL::ShaderDataType::Mat3:
            switch (uniform.Settings.Type) {
                case ImGuiWidgetType::Input:
                    ImGui::InputFloat3("##InputFloat3", (float*) data);
                    break;
                case ImGuiWidgetType::Drag:
                    if (uniform.Settings.UseRange)
                        ImGui::DragFloat3("##DragFloat3", (float*) data, uniform.Settings.Speed, uniform.Settings.Min, uniform.Settings.Max);
                    else
                        ImGui::DragFloat3("##DragFloat3", (float*) data, uniform.Settings.Speed);
                    break;
                case ImGuiWidgetType::Slider:
                    ImGui::SliderFloat3("##SliderFloat3", (float*) data, uniform.Settings.Min, uniform.Settings.Max);
                    break;
                case ImGuiWidgetType::Color:
                    ImGui::ColorEdit3("##ColorFloat3", (float*) data);
                    break;
            }
            break;
        case OpenGL::ShaderDataType::Mat4:
            switch (uniform.Settings.Type) {
                case ImGuiWidgetType::Input:
                    ImGui::InputFloat4("##InputFloat4", (float*) data);
                    break;
                case ImGuiWidgetType::Drag:
                    if (uniform.Settings.UseRange)
                        ImGui::DragFloat4("##DragFloat4", (float*) data, uniform.Settings.Speed, uniform.Settings.Min, uniform.Settings.Max);
                    else
                        ImGui::DragFloat4("##DragFloat4", (float*) data, uniform.Settings.Speed);
                    break;
                case ImGuiWidgetType::Slider:
                    ImGui::SliderFloat4("##SliderFloat4", (float*) data, uniform.Settings.Min, uniform.Settings.Max);
                    break;
                case ImGuiWidgetType::Color:
                    ImGui::ColorEdit4("##ColorFloat4", (float*) data);
                    break;
            }
            break;
        default:
            return;
    }
}

void UniformView::DrawInputInt(Uniform& uniform, void* data) {
    switch (uniform.Type) {
        case OpenGL::ShaderDataType::Int:
            switch (uniform.Settings.Type) {
                case ImGuiWidgetType::Input:
                    ImGui::InputInt("##InputInt", (int*) data);
                    break;
                case ImGuiWidgetType::Drag:
                    ImGui::DragInt("##DragInt", (int*) data);
                    break;
                case ImGuiWidgetType::Slider:
                    ImGui::SliderInt("##SliderInt", (int*) data, uniform.Settings.Min, uniform.Settings.Max);
                    break;
                case ImGuiWidgetType::Color:
                    break;
            }
            break;
        case OpenGL::ShaderDataType::Int2:
            switch (uniform.Settings.Type) {
                case ImGuiWidgetType::Input:
                    ImGui::InputInt2("##InputInt2", (int*) data);
                    break;
                case ImGuiWidgetType::Drag:
                    ImGui::DragInt2("##DragInt2", (int*) data);
                    break;
                case ImGuiWidgetType::Slider:
                    ImGui::SliderInt2("##SliderInt2", (int*) data, uniform.Settings.Min, uniform.Settings.Max);
                    break;
                case ImGuiWidgetType::Color:
                    break;
            }
            break;
        case OpenGL::ShaderDataType::Int3:
            switch (uniform.Settings.Type) {
                case ImGuiWidgetType::Input:
                    ImGui::InputInt3("##InputInt3", (int*) data);
                    break;
                case ImGuiWidgetType::Drag:
                    ImGui::DragInt3("##DragInt3", (int*) data);
                    break;
                case ImGuiWidgetType::Slider:
                    ImGui::SliderInt3("##SliderInt3", (int*) data, uniform.Settings.Min, uniform.Settings.Max);
                    break;
                case ImGuiWidgetType::Color:
                    break;
            }
            break;
        case OpenGL::ShaderDataType::Int4:
            switch (uniform.Settings.Type) {
                case ImGuiWidgetType::Input:
                    ImGui::InputInt4("##InputInt4", (int*) data);
                    break;
                case ImGuiWidgetType::Drag:
                    ImGui::DragInt4("##DragInt4", (int*) data);
                    break;
                case ImGuiWidgetType::Slider:
                    ImGui::SliderInt4("##SliderInt4", (int*) data, uniform.Settings.Min, uniform.Settings.Max);
                    break;
                case ImGuiWidgetType::Color:
                    break;
            }
            break;
        default:
            return;
    }
}

}
