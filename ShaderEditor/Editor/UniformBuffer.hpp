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

#include "ImGuiInputSettings.h"

namespace Editor {

struct Uniform {
    std::string Title;
    OpenGL::ShaderDataType Type;
    
    InputSettings Settings;

    Uniform(const std::string& title = "New Uniform",
            OpenGL::ShaderDataType type = OpenGL::ShaderDataType::Float)
    : Title(title), Type(type) {}
    
    uint32_t Size() { return OpenGL::ShaderDataTypeSize(Type); }
};

class UniformBuffer {
public:
    UniformBuffer();
    
    ~UniformBuffer();
    
    void PushUniform(OpenGL::ShaderDataType type = OpenGL::ShaderDataType::Float);
    void ChangeType(int index, OpenGL::ShaderDataType type);
    
    size_t Count() { return m_Uniforms.size(); }
    
    Uniform& GetUniform(int index) { return m_Uniforms[index]; }
    char* GetUniformData(int index) { return GetData(GetDataOffsetForIndex(index)); }
    
    void Delete(int index) { DeleteDataAtIndex(index); m_Uniforms.erase(m_Uniforms.begin() + index);}
    
    Uniform& operator[](int index) { return GetUniform(index); }
    
private:
    char* GetData(int offset) { return &m_Buffer[offset]; }
    uint32_t GetDataOffsetForIndex(int index);
    void DeleteDataAtIndex(int index);
    
private:
    std::vector<Uniform> m_Uniforms;
    std::vector<char> m_Buffer;
};

}

#endif /* UniformBuffer_hpp */
