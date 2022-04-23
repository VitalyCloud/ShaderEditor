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
        switch(Type)
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
    VertexBuffer(float* vertices, uint32_t size);
    ~VertexBuffer();
    
    void SetLayout(const BufferLayout& layout) { m_Layout = layout; }
    const BufferLayout& GetLayout() const { return m_Layout; }
    
    void Bind();
    void Unbind();
    
private:
    uint32_t m_RendererID;
    BufferLayout m_Layout;
};

class IndexBuffer {
public:
    IndexBuffer(uint32_t* indices, uint32_t count);
    ~IndexBuffer();
    
    void Bind();
    void Unbind();
    
    uint32_t GetCount() const { return m_Count; }
private:
    uint32_t m_RendererID;
    uint32_t m_Count;
};


}


#endif /* Buffer_hpp */
