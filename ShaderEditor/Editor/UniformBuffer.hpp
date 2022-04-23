//
//  UniformBuffer.hpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 23.04.2022.
//

#ifndef UniformBuffer_hpp
#define UniformBuffer_hpp

#include <vector>
#include "OpenGL/Buffer.hpp"

namespace Editor {

struct Uniform {
    std::string Title;
    OpenGL::ShaderDataType Type;
    
    Uniform(const std::string& title = "New Uniform",
            OpenGL::ShaderDataType type = OpenGL::ShaderDataType::Float)
    : Title(title), Type(type) {}
    
    uint32_t Size() {
        return OpenGL::ShaderDataTypeSize(Type);
    }
};

class UniformBuffer {
public:
    UniformBuffer() {
        auto floatSize = OpenGL::ShaderDataTypeSize(OpenGL::ShaderDataType::Float);
        auto size = floatSize * 10;
        m_Buffer.reserve(size);
    }
    
    ~UniformBuffer() {}
    
    size_t Size() { return m_Uniforms.size(); }
    
    void PushUnifrom() {
        m_Uniforms.emplace_back();
        auto floatSize = OpenGL::ShaderDataTypeSize(OpenGL::ShaderDataType::Float);
        m_Buffer.resize(m_Buffer.size() + floatSize, 0);
    }
    
    void PushUniform(OpenGL::ShaderDataType type) {
        m_Uniforms.emplace_back("New Uniform", type);
        auto typeSize = OpenGL::ShaderDataTypeSize(type);
        m_Buffer.resize(m_Buffer.size() + typeSize, 0);
    }
    
    Uniform& GetUniform(int index) {
        return m_Uniforms[index];
    }
    
    char* GetUniformData(int index) {
        int offset = GetDataOffsetForIndex(index);
        return GetData(offset);
    }
    
    char* GetData(int offset) {
        return &m_Buffer[offset];
    }
    
    void Delete(int index) {
        int offset = GetDataOffsetForIndex(index);
        uint32_t dataSize = OpenGL::ShaderDataTypeSize(m_Uniforms[index].Type);
        m_Buffer.erase(m_Buffer.begin() + offset, m_Buffer.begin() + offset + dataSize);
        m_Uniforms.erase(m_Uniforms.begin() + index);
    }
    
private:
    uint32_t GetDataOffsetForIndex(int index) {
        uint32_t offset = 0;
        for(size_t i=0; i<m_Uniforms.size(); i++) {
            if(i == index)
                break;
            offset += m_Uniforms[i].Size();
        }
        return offset;
    }
    
private:
    std::vector<Uniform> m_Uniforms;
    std::vector<char> m_Buffer;
};

}

#endif /* UniformBuffer_hpp */
