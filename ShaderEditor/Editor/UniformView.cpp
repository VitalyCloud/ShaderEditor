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
//    m_Uniforms.PushUnifrom();
//    m_Uniforms.PushUniform(OpenGL::ShaderDataType::Float2);
//    m_Uniforms.PushUniform(OpenGL::ShaderDataType::Float3);
//    m_Uniforms.PushUniform(OpenGL::ShaderDataType::Float4);
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
            ImGui::PushItemWidth(ImGui::GetColumnWidth());
            DrawUniformTypeInput(uniform.Type, m_Uniforms.GetUniformData(i));
            ImGui::PopItemWidth();
            
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

void UniformView::DrawUniformTypeInput(const OpenGL::ShaderDataType& type, void* data) {
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

}
