//
//  Shader.hpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 21.04.2022.
//

#ifndef Shader_hpp
#define Shader_hpp

#include <string>
#include "glm/glm.hpp"

namespace OpenGL {

struct ShaderProgramSource{
    std::string VertexSource;
    std::string FragmentSource;
};

class Shader {
public:
    Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
    ~Shader();
    
    void Bind();
    void Unbind();
    
    bool SetInt(const std::string& name, int value);
    bool SetInt2(const std::string& name, const int* value);
    bool SetInt3(const std::string& name, const int* value);
    bool SetInt4(const std::string& name, const int* value);
    bool SetIntArray(const std::string& name, int* values, uint32_t count);
    bool SetFloat(const std::string& name, float value);
    bool SetFloat2(const std::string& name, const glm::vec2& value);
    bool SetFloat2(const std::string& name, const float* value);
    bool SetFloat3(const std::string& name, const glm::vec3& value);
    bool SetFloat3(const std::string& name, const float* value);
    bool SetFloat4(const std::string& name, const glm::vec4& value);
    bool SetFloat4(const std::string& name, const float* value);
    bool SetFloatArray(const std::string& name, float* values, uint32_t count);
    bool SetMat3(const std::string& name, const glm::mat3& value);
    bool SetMat3(const std::string& name, const float* values);
    bool SetMat4(const std::string& name, const glm::mat4& value);
    bool SetMat4(const std::string& name, const float* values);
    
    bool HasError() { return !m_ErrorMessage.empty(); }
    const std::string& GetErrorMessage() { return m_ErrorMessage; }
    
private:
    unsigned int m_RendererID;
    std::string m_ErrorMessage;
};


}




#endif /* Shader_hpp */
