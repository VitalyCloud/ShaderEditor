//
//  UniformBufferContainer.hpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 01.05.2022.
//

#ifndef UniformBufferContainer_hpp
#define UniformBufferContainer_hpp

#include <vector>

#include "Core/Memory.hpp"
#include "OpenGL/Buffer.hpp"
#include "OpenGL/Shader.hpp"
#include "ImGuiHelper.hpp"

#include "glm/glm.hpp"

namespace Editor {

struct Uniform {
    OpenGL::ShaderDataType Type;
    std::string Title;
    bool Connected = false;
    ImGuiInputSettings Settings;
  
    Uniform(OpenGL::ShaderDataType type = OpenGL::ShaderDataType::Float,
            const std::string& title = "NewUniform")
    : Title(title), Type(type) {}
    
    uint32_t Size() { return OpenGL::ShaderDataTypeSize(Type); }
};

class UniformBufferContainer {
public:
    using UniformData = char;
    
    UniformBufferContainer();
    ~UniformBufferContainer();
    
    size_t Count() { return m_Uniforms.size(); }
    
    void PushUniform(OpenGL::ShaderDataType type = OpenGL::ShaderDataType::Float, const std::string& title = "NewUniform", UniformData* value = nullptr, int size = 0);
    void PopUniform(int index);
    
    void SetUniform(int index, UniformData* value, uint32_t size);
    void SetUniform(int index, float value);
    void SetUniform(int index, int value);
    void SetUniform(int index, const glm::vec2& value);
    void SetUniform(int index, const glm::vec3& value);
    void SetUniform(int index, const glm::vec4& value);
    void SetUniform(int index, const glm::mat3& value);
    void SetUniform(int index, const glm::mat4& value);
    
    void ChangeUniformType(int index, OpenGL::ShaderDataType type);
    
    Uniform& GetUniform(int index) { return m_Uniforms[index]; }
    UniformData* GetUniformData(int index) { return &m_Data[GetDataOffsetForIndex(index)]; }
    
    void UploadUniforms(const Core::Ref<OpenGL::Shader>& shader);
    
    Uniform& operator[](int index) { return GetUniform(index); }
    
private:
    UniformData* GetData(int offset) { return &m_Data[offset]; }
    uint32_t GetDataOffsetForIndex(int index);
    void DeleteDataAtIndex(int index);
    
private:
    std::vector<UniformData> m_Data;
    std::vector<Uniform> m_Uniforms;
};

}

#endif /* UniformBufferContainer_hpp */
