//
//  Buffer.cpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 21.04.2022.
//
#include "pch.h"
#include "Buffer.hpp"
#include "glad/glad.h"

namespace OpenGL {

uint32_t ShaderDataTypeSize(ShaderDataType type)
{
    switch (type)
    {
        case ShaderDataType::None:      break;
        case ShaderDataType::Float:     return 4;
        case ShaderDataType::Float2:    return 4 * 2;
        case ShaderDataType::Float3:    return 4 * 3;
        case ShaderDataType::Float4:    return 4 * 4;
        case ShaderDataType::Mat3:      return 4 * 3 * 3;
        case ShaderDataType::Mat4:      return 4 * 4 * 4;
        case ShaderDataType::Int:       return 4;
        case ShaderDataType::Int2:      return 4 * 2;
        case ShaderDataType::Int3:      return 4 * 3;
        case ShaderDataType::Int4:      return 4 * 4;
        case ShaderDataType::Bool:      return 1;
    }
    
    EN_CORE_ASSERT(false, "Unknown ShaderDataType!");
    return 0;
}

const char* ShaderDataTypeTitle(ShaderDataType type) {
    switch (type)
    {
        case ShaderDataType::None:      return "None";
        case ShaderDataType::Float:     return "Float";
        case ShaderDataType::Float2:    return "Float2";
        case ShaderDataType::Float3:    return "Float3";
        case ShaderDataType::Float4:    return "Float4";
        case ShaderDataType::Mat3:      return "Mat3";
        case ShaderDataType::Mat4:      return "Mat4";
        case ShaderDataType::Int:       return "Int";
        case ShaderDataType::Int2:      return "Int2";
        case ShaderDataType::Int3:      return "Int3";
        case ShaderDataType::Int4:      return "Int4";
        case ShaderDataType::Bool:      return "Bool";
    }
    
    EN_CORE_ASSERT(false, "Unknown ShaderDataType!");
    return 0;
}

const uint32_t ShaderDataTypeCount() {
    return 12;
}

uint32_t ShaderDataTypeComponentCount(ShaderDataType type) {
    switch(type)
    {
        case ShaderDataType::None:      break;
        case ShaderDataType::Float:     return 1;
        case ShaderDataType::Float2:    return 2;
        case ShaderDataType::Float3:    return 3;
        case ShaderDataType::Float4:    return 4;
        case ShaderDataType::Mat3:      return 3 * 3;
        case ShaderDataType::Mat4:      return 4 * 4;
        case ShaderDataType::Int:       return 1;
        case ShaderDataType::Int2:      return 2;
        case ShaderDataType::Int3:      return 3;
        case ShaderDataType::Int4:      return 4;
        case ShaderDataType::Bool:      return 1;
    }
    
    EN_CORE_ASSERT(false, "Unknown ShaderDataType!");
    return 0;
}

uint32_t VertexBuffer::s_ActviveObjectsCount = 0;

VertexBuffer::VertexBuffer(float *vertices, uint32_t size) { 
    glGenBuffers(1, &m_RendererID);
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    m_Size = size;
    EN_INFO("Vertex Buffer {0} created, Size: {1}", (uint64_t)this, size);
    s_ActviveObjectsCount+=1;
}

VertexBuffer::VertexBuffer(uint32_t size) {
    glGenBuffers(1, &m_RendererID);
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_STATIC_DRAW);
    m_Size = size;
    EN_INFO("Vertex Buffer {0} created, Size: {1}", (uint64_t)this, size);
    s_ActviveObjectsCount+=1;
}

void VertexBuffer::Resize(uint32_t size) {
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
    EN_INFO("Vertex Buffer {0} resized from {1} to {2}", (uint64_t)this, m_Size, size);
    m_Size = size;
}

void VertexBuffer::SetData(const void* data, uint32_t size) {
    EN_ASSERT(m_Size <= size);
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}

VertexBuffer::~VertexBuffer() { 
    glDeleteBuffers(1, &m_RendererID);
    EN_WARN("Vertex Buffer {0} deleted", (uint64_t)this);
    s_ActviveObjectsCount-=1;
}

void VertexBuffer::Bind() { 
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}

void VertexBuffer::Unbind() { 
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

uint32_t IndexBuffer::s_ActviveObjectsCount = 0;

IndexBuffer::IndexBuffer(uint32_t *indices, uint32_t count) : m_Count(count) {
    glGenBuffers(1, &m_RendererID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
    EN_INFO("Index Buffer {0} created, Count: {1}", (uint64_t)this, count);
    s_ActviveObjectsCount += 1;
}

IndexBuffer::IndexBuffer(uint32_t count) : m_Count(count) {
    glGenBuffers(1, &m_RendererID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), nullptr, GL_STATIC_DRAW);
    EN_INFO("Index Buffer {0} created, Count: {1}", (uint64_t)this, count);
    s_ActviveObjectsCount += 1;
}

void IndexBuffer::Resize(uint32_t count) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), nullptr, GL_DYNAMIC_DRAW);
    EN_INFO("Index Buffer {0} resized from {1} to {2}", (uint64_t)this, m_Count, count);
    m_Count = count;
    
}

void IndexBuffer::SetData(uint32_t* indices, uint32_t count) {
    EN_ASSERT(count <= m_Count);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, count * sizeof(uint32_t), indices);
}

IndexBuffer::~IndexBuffer() { 
    glDeleteBuffers(1, &m_RendererID);
    EN_INFO("Index Buffer {0} deleted", (uint64_t)this);
    s_ActviveObjectsCount -= 1;
}

void IndexBuffer::Bind() { 
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
}

void IndexBuffer::Unbind() { 
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

}
