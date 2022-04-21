//
//  Buffer.cpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 21.04.2022.
//

#include "Buffer.hpp"
#include "glad/glad.h"

namespace OpenGL {

VertexBuffer::VertexBuffer(float *vertices, uint32_t size) { 
    glGenBuffers(1, &m_RendererID);
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer() { 
    glDeleteBuffers(1, &m_RendererID);
}

void VertexBuffer::Bind() { 
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}

void VertexBuffer::Unbind() { 
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


IndexBuffer::IndexBuffer(uint32_t *indices, uint32_t count) : m_Count(count) {
    glGenBuffers(1, &m_RendererID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer() { 
    glDeleteBuffers(1, &m_RendererID);
}

void IndexBuffer::Bind() { 
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
}

void IndexBuffer::Unbind() { 
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

}
