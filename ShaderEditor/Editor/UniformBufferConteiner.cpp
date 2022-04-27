//
//  UniformBuffer.cpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 23.04.2022.
//
#include "Core/pch.h"
#include "UniformBufferConteiner.hpp"

namespace Editor {

UniformBufferConteiner::UniformBufferConteiner() {
    auto floatSize = OpenGL::ShaderDataTypeSize(OpenGL::ShaderDataType::Float);
    auto size = floatSize * 10;
    m_Buffer.reserve(size);
}

UniformBufferConteiner::~UniformBufferConteiner() {}

void UniformBufferConteiner::PushUniform(OpenGL::ShaderDataType type, const std::string& title) {
    m_Uniforms.emplace_back(title, type);
    auto typeSize = OpenGL::ShaderDataTypeSize(type);
    m_Buffer.resize(m_Buffer.size() + typeSize, 0);
}

void UniformBufferConteiner::SetUniformData(int index, const void* data) {
    uint32_t size = m_Uniforms[index].Size();
    uint32_t begin = GetDataOffsetForIndex(index);
    char* dataPtr = (char*)data;
    for(int i=0; i<size; i++) {
        m_Buffer[begin+i] = *(dataPtr + i);
    }
}

void UniformBufferConteiner::SetUniformData(const std::string& title, const void* data) {
    int index = -1;
    for(int i=0; i<m_Uniforms.size(); i++) {
        if(m_Uniforms[i].Title == title) {
            index = i;
            break;
        }
    }
    
    if(index >= 0)
        SetUniformData(index, data);
}

void UniformBufferConteiner::ChangeType(int index, OpenGL::ShaderDataType type) {
    if(m_Uniforms[index].Type == type) { return; }
    DeleteDataAtIndex(index);
    int offset = GetDataOffsetForIndex(index);
    uint32_t newTypeSize = OpenGL::ShaderDataTypeSize(type);
    m_Buffer.insert(m_Buffer.begin() + offset, newTypeSize, 0);
    m_Uniforms[index].Type = type;
}

uint32_t UniformBufferConteiner::GetDataOffsetForIndex(int index) {
    uint32_t offset = 0;
    for(size_t i=0; i<m_Uniforms.size(); i++) {
        if(i == index)
            break;
        offset += m_Uniforms[i].Size();
    }
    return offset;
}

void UniformBufferConteiner::DeleteDataAtIndex(int index) {
    int offset = GetDataOffsetForIndex(index);
    uint32_t dataSize = OpenGL::ShaderDataTypeSize(m_Uniforms[index].Type);
    m_Buffer.erase(m_Buffer.begin() + offset, m_Buffer.begin() + offset + dataSize);
}

void UniformBufferConteiner::UploadUniforms(const Core::Ref<OpenGL::Shader>& shader) {
    uint32_t offset = 0;
    shader->Bind();
    for(auto& uniform: m_Uniforms) {
        auto value = m_Buffer.data() + offset;
        float* floatValue = (float*)value;
        int* intValue = (int*)value;
        
        switch (uniform.Type)
        {
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
                uniform.Connected = shader->SetMat3v(uniform.Title, floatValue);
                break;
            case OpenGL::ShaderDataType::Mat4:
                uniform.Connected = shader->SetMat4v(uniform.Title, floatValue);
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
            case OpenGL::ShaderDataType::Bool:      break;
            default: break;
        }
        offset+=uniform.Size();
    }
    shader->Unbind();
}


}
