//
//  Buffer.hpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 21.04.2022.
//

#ifndef Buffer_hpp
#define Buffer_hpp

#include <string>
#include <assert.h>
#include <vector>

#include "Core/Assert.h"

namespace OpenGL {

enum class ShaderDataType
{
    None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
};

uint32_t ShaderDataTypeSize(ShaderDataType type);
const char* ShaderDataTypeTitle(ShaderDataType type);
const uint32_t ShaderDataTypeCount();
uint32_t ShaderDataTypeComponentCount(ShaderDataType type);

struct BufferElement
{
    std::string Name;
    ShaderDataType Type;
    uint32_t Size;
    uint32_t Offset;
    bool Normalized;
    
    BufferElement() {}
    
    BufferElement(ShaderDataType type, const std::string& name, bool normalized = false)
        : Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(normalized) {
        
    }
    
    uint32_t GetComponentCount() const
    {
        return ShaderDataTypeComponentCount(Type);
    }

    
};

class BufferLayout
{
public:
    
    BufferLayout() {}
    
    BufferLayout(const std::initializer_list<BufferElement>& elements)
        : m_Elements(elements)
    {
        CalculateOffsetAndStride();
    }

    BufferLayout(const std::vector<BufferElement>& elements)
        : m_Elements(elements)
    {
        CalculateOffsetAndStride();
    }
    
    inline uint32_t GetStride() const { return m_Stride; }
    inline const std::vector<BufferElement>& GetElements() const { return m_Elements; }
    
    std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
    std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
    std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
    std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }
private:
    void CalculateOffsetAndStride()
    {
        uint32_t offset = 0;
        m_Stride = 0;
        for(auto& element : m_Elements)
        {
            element.Offset = offset;
            offset += element.Size;
            m_Stride += element.Size;
        }
    }
private:
    std::vector<BufferElement> m_Elements;
    uint32_t m_Stride = 0;
};

class VertexBuffer {
public:
    VertexBuffer(uint32_t size);
    VertexBuffer(float* vertices, uint32_t size);
    ~VertexBuffer();
    
    void Resize(uint32_t size);
    void SetData(const void* data, uint32_t size);
    void SetLayout(const BufferLayout& layout) { m_Layout = layout; }
    const BufferLayout& GetLayout() const { return m_Layout; }
    
    void Bind();
    void Unbind();
    
    static uint32_t GetActiveObjectsCount() { return s_ActviveObjectsCount; }
private:
    static uint32_t s_ActviveObjectsCount;
    
    uint32_t m_RendererID;
    BufferLayout m_Layout;
    uint32_t m_Size;
};

class IndexBuffer {
public:
    IndexBuffer(uint32_t* indices, uint32_t count);
    IndexBuffer(uint32_t count);
    ~IndexBuffer();
    
    void SetData(uint32_t* indices, uint32_t count);
    void Resize(uint32_t count);
    
    void Bind();
    void Unbind();
    
    uint32_t GetCount() const { return m_Count; }
    
    static uint32_t GetActiveObjectsCount() { return s_ActviveObjectsCount; }
private:
    static uint32_t s_ActviveObjectsCount;
    
    uint32_t m_RendererID;
    uint32_t m_Count;
};


}


#endif /* Buffer_hpp */
