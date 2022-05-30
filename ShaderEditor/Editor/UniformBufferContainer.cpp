//
//  UniformBufferContainer.cpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 01.05.2022.
//
#include "pch.h"
#include "UniformBufferContainer.hpp"

#include "glm/gtc/type_ptr.hpp"

namespace Editor {

UniformBufferContainer::UniformBufferContainer() {
    
}

UniformBufferContainer::~UniformBufferContainer() {
    
}

void UniformBufferContainer::PushUniform(OpenGL::ShaderDataType type, const std::string& title, UniformData* value, int size) {
    m_Uniforms.emplace_back(type, title);
    auto typeSize = OpenGL::ShaderDataTypeSize(type);
    m_Data.resize(m_Data.size() + typeSize, 0);
    
    if(value != nullptr)
        SetUniform((int)m_Uniforms.size()-1, value, size);
}

void UniformBufferContainer::PopUniform(int index) {
    EN_ASSERT(index < m_Data.size(), "Index out of range");
    DeleteDataAtIndex(index);
    m_Uniforms.erase(m_Uniforms.begin() + index);
}

void UniformBufferContainer::SetUniform(int index, UniformData* value, uint32_t size) {
    EN_ASSERT(value != nullptr, "Uniform data is null");
    EN_ASSERT(m_Uniforms[index].Size() == size, "Uniform sizes don't match");
    EN_ASSERT(index < m_Data.size(), "Index out of range");
    
    uint32_t begin = GetDataOffsetForIndex(index);
    for(int i=0; i<size; i++)
        m_Data[begin+i] = *(value + i);
}

void UniformBufferContainer::ChangeUniformType(int index, OpenGL::ShaderDataType type) {
    EN_ASSERT(index < m_Data.size(), "Index out of range");
    if(m_Uniforms[index].Type == type) { return; }
    
    DeleteDataAtIndex(index);
    int offset = GetDataOffsetForIndex(index);
    m_Data.insert(m_Data.begin() + offset, OpenGL::ShaderDataTypeSize(type), 0);
    m_Uniforms[index].Type = type;
}

void UniformBufferContainer::UploadUniforms(const Core::Ref<OpenGL::Shader>& shader) {
    
    uint32_t offset = 0;
    shader->Bind();
    
    for(auto& uniform: m_Uniforms) {
        auto value = m_Data.data() + offset;
        float* floatValue = (float*)value;
        int* intValue  = (int*)value;
        
        switch (uniform.Type) {
            case OpenGL::ShaderDataType::Float:
                uniform.Connected = shader->SetFloat(uniform.Title, *floatValue);
                break;
            case OpenGL::ShaderDataType::Float2:
                uniform.Connected = shader->SetFloat2(uniform.Title, floatValue);
                break;
            case OpenGL::ShaderDataType::Float3:
                uniform.Connected = shader->SetFloat3(uniform.Title, floatValue);
                break;
            case OpenGL::ShaderDataType::Float4:
                uniform.Connected = shader->SetFloat4(uniform.Title, floatValue);
                break;
            case OpenGL::ShaderDataType::Mat3:
                uniform.Connected = shader->SetMat3(uniform.Title, floatValue);
                break;
            case OpenGL::ShaderDataType::Mat4:
                uniform.Connected = shader->SetMat4(uniform.Title, floatValue);
                break;
            case OpenGL::ShaderDataType::Int:
                uniform.Connected = shader->SetInt(uniform.Title, *intValue);
                break;
            case OpenGL::ShaderDataType::Int2:
                uniform.Connected = shader->SetInt2(uniform.Title, intValue);
                break;
            case OpenGL::ShaderDataType::Int3:
                uniform.Connected = shader->SetInt3(uniform.Title, intValue);
                break;
            case OpenGL::ShaderDataType::Int4:
                uniform.Connected = shader->SetInt4(uniform.Title, intValue);
                break;
            case OpenGL::ShaderDataType::Bool:
                break;
            default:
                break;
        }
        offset += uniform.Size();
    }
    
    shader->Unbind();
    
}

uint32_t UniformBufferContainer::GetDataOffsetForIndex(int index) {
//    EN_ASSERT(index < m_Data.size(), "Index out of range");
    uint32_t offset = 0;
    for(size_t i=0; i<m_Uniforms.size(); i++) {
        if(i == index)
            break;
        offset += m_Uniforms[i].Size();
    }
    return offset;
}

void UniformBufferContainer::DeleteDataAtIndex(int index) {
    EN_ASSERT(index < m_Data.size(), "Index out of range");
    int offset = GetDataOffsetForIndex(index);
    m_Data.erase(m_Data.begin() + offset, m_Data.begin() + offset + OpenGL::ShaderDataTypeSize(m_Uniforms[index].Type));
}

void UniformBufferContainer::SetUniform(int index, float value) {
    EN_ASSERT(index < m_Data.size(), "Index out of range");
    EN_ASSERT(m_Uniforms[index].Type == OpenGL::ShaderDataType::Float);
    SetUniform(index, (UniformData*)(void*)&value, sizeof(float));
}

void UniformBufferContainer::SetUniform(int index, int value) {
    EN_ASSERT(index < m_Data.size(), "Index out of range");
    EN_ASSERT(m_Uniforms[index].Type == OpenGL::ShaderDataType::Int);
    SetUniform(index, (UniformData*)(void*)&value, sizeof(float));
}

void UniformBufferContainer::SetUniform(int index, const glm::vec2& value) {
    EN_ASSERT(index < m_Data.size(), "Index out of range");
    EN_ASSERT(m_Uniforms[index].Type == OpenGL::ShaderDataType::Float2);
    SetUniform(index, (UniformData*)glm::value_ptr(value), sizeof(value));
}

void UniformBufferContainer::SetUniform(int index, const glm::vec3& value) {
    EN_ASSERT(index < m_Data.size(), "Index out of range");
    EN_ASSERT(m_Uniforms[index].Type == OpenGL::ShaderDataType::Float3);
    SetUniform(index, (UniformData*)glm::value_ptr(value), sizeof(value));
}

void UniformBufferContainer::SetUniform(int index, const glm::vec4& value) {
    EN_ASSERT(index < m_Data.size(), "Index out of range");
    EN_ASSERT(m_Uniforms[index].Type == OpenGL::ShaderDataType::Float4);
    SetUniform(index, (UniformData*)glm::value_ptr(value), sizeof(value));
}

void UniformBufferContainer::SetUniform(int index, const glm::mat3& value) {
    EN_ASSERT(index < m_Data.size(), "Index out of range");
    EN_ASSERT(m_Uniforms[index].Type == OpenGL::ShaderDataType::Mat3);
    SetUniform(index, (UniformData*)glm::value_ptr(value), sizeof(value));
}

void UniformBufferContainer::SetUniform(int index, const glm::mat4& value) {
    EN_ASSERT(index < m_Data.size(), "Index out of range");
    EN_ASSERT(m_Uniforms[index].Type == OpenGL::ShaderDataType::Mat4);
    SetUniform(index, (UniformData*)glm::value_ptr(value), sizeof(value));
}


}
