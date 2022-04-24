//
//  UniformBuffer.cpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 23.04.2022.
//
#include "Core/pch.h"
#include "UniformBuffer.hpp"

namespace Editor {

UniformBuffer::UniformBuffer() {
    auto floatSize = OpenGL::ShaderDataTypeSize(OpenGL::ShaderDataType::Float);
    auto size = floatSize * 10;
    m_Buffer.reserve(size);
}

UniformBuffer::~UniformBuffer() {}

void UniformBuffer::PushUniform(OpenGL::ShaderDataType type) {
    m_Uniforms.emplace_back("New Uniform", type);
    auto typeSize = OpenGL::ShaderDataTypeSize(type);
    m_Buffer.resize(m_Buffer.size() + typeSize, 0);
}

void UniformBuffer::ChangeType(int index, OpenGL::ShaderDataType type) {
    if(m_Uniforms[index].Type == type) { return; }
    DeleteDataAtIndex(index);
    int offset = GetDataOffsetForIndex(index);
    uint32_t newTypeSize = OpenGL::ShaderDataTypeSize(type);
    m_Buffer.insert(m_Buffer.begin() + offset, newTypeSize, 0);
    m_Uniforms[index].Type = type;
}

uint32_t UniformBuffer::GetDataOffsetForIndex(int index) {
    uint32_t offset = 0;
    for(size_t i=0; i<m_Uniforms.size(); i++) {
        if(i == index)
            break;
        offset += m_Uniforms[i].Size();
    }
    return offset;
}

void UniformBuffer::DeleteDataAtIndex(int index) {
    int offset = GetDataOffsetForIndex(index);
    uint32_t dataSize = OpenGL::ShaderDataTypeSize(m_Uniforms[index].Type);
    m_Buffer.erase(m_Buffer.begin() + offset, m_Buffer.begin() + offset + dataSize);
}


}
