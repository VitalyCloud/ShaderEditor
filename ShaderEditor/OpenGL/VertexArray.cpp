//
//  VertexArray.cpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 21.04.2022.
//
#include "pch.h"
#include "VertexArray.hpp"
#include "glad/glad.h"



namespace OpenGL {


static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {
    switch (type)
    {
        case ShaderDataType::None:     break;
        case ShaderDataType::Float:    return GL_FLOAT;
        case ShaderDataType::Float2:   return GL_FLOAT;
        case ShaderDataType::Float3:   return GL_FLOAT;
        case ShaderDataType::Float4:   return GL_FLOAT;
        case ShaderDataType::Mat3:     return GL_FLOAT;
        case ShaderDataType::Mat4:     return GL_FLOAT;
        case ShaderDataType::Int:      return GL_INT;
        case ShaderDataType::Int2:     return GL_INT;
        case ShaderDataType::Int3:     return GL_INT;
        case ShaderDataType::Int4:     return GL_INT;
        case ShaderDataType::Bool:     return GL_BOOL;
        
    }

    EN_CORE_ASSERT(false, "Unknown ShaderDataType!");
    return 0;
}

uint32_t VertexArray::s_ActviveObjectsCount = 0;

VertexArray::VertexArray() { 
    glGenVertexArrays(1, &m_RendererID);
    EN_INFO("Vertex Array created: {0}", (u_int64_t)this);
    s_ActviveObjectsCount += 1;
}

VertexArray::~VertexArray() { 
    glDeleteVertexArrays(1, &m_RendererID);
    EN_WARN("Vertex Array deleted: {0}", (u_int64_t)this);
    s_ActviveObjectsCount -= 1;
}

void VertexArray::Bind() { 
    glBindVertexArray(m_RendererID);
}

void VertexArray::Unbind() { 
    glBindVertexArray(0);
}

void VertexArray::AddVertexBuffer(const Core::Ref<VertexBuffer> &vertexBuffer) { 
    
    assert(vertexBuffer->GetLayout().GetElements().size()); // "Vertex Buffer has no layout!"
    
    glBindVertexArray(m_RendererID);
    vertexBuffer->Bind();
    const auto& layout = vertexBuffer->GetLayout();
    
    for(const auto& element: layout) {
        switch (element.Type) {
            case ShaderDataType::Float:
            case ShaderDataType::Float2:
            case ShaderDataType::Float3:
            case ShaderDataType::Float4: {
                glEnableVertexAttribArray(m_VertexBufferIndex);
                glVertexAttribPointer(m_VertexBufferIndex,
                    element.GetComponentCount(),
                    ShaderDataTypeToOpenGLBaseType(element.Type),
                    element.Normalized ? GL_TRUE : GL_FALSE,
                    layout.GetStride(),
                    (void*)(uintptr_t)element.Offset);
                m_VertexBufferIndex++;
                break;
            }
            case ShaderDataType::Int:
            case ShaderDataType::Int2:
            case ShaderDataType::Int3:
            case ShaderDataType::Int4:
            case ShaderDataType::Bool: {
                glEnableVertexAttribArray(m_VertexBufferIndex);
                glVertexAttribIPointer(m_VertexBufferIndex,
                    element.GetComponentCount(),
                    ShaderDataTypeToOpenGLBaseType(element.Type),
                    layout.GetStride(),
                    (void*)(uintptr_t)element.Offset);
                m_VertexBufferIndex++;
                break;
            }
            case ShaderDataType::Mat3:
            case ShaderDataType::Mat4: {
                uint8_t count = element.GetComponentCount();
                for (uint8_t i = 0; i < count; i++) {
                    glEnableVertexAttribArray(m_VertexBufferIndex);
                    glVertexAttribPointer(m_VertexBufferIndex,
                        count,
                        ShaderDataTypeToOpenGLBaseType(element.Type),
                        element.Normalized ? GL_TRUE : GL_FALSE,
                        layout.GetStride(),
                        (void*)(uintptr_t)(element.Offset + sizeof(float) * count * i));
                    glVertexAttribDivisor(m_VertexBufferIndex, 1);
                    m_VertexBufferIndex++;
                }
                break;
            }
            default:
                EN_CORE_ASSERT(false, "Unknown ShaderDataType!");
        }
    }
    m_VertexBuffers.push_back(vertexBuffer);
}

void VertexArray::SetIndexBuffer(const Core::Ref<IndexBuffer> &indexBuffer) { 
    glBindVertexArray(m_RendererID);
    indexBuffer->Bind();
    m_IndexBuffer = indexBuffer;
}

}
