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
    
    void SetInt(const std::string& name, int value);
    void SetIntArray(const std::string& name, int* values, uint32_t count);
    void SetFloat(const std::string& name, float value);
    void SetFloat2(const std::string& name, const glm::vec2& value);
    void SetFloat3(const std::string& name, const glm::vec3& value);
    void SetFloat4(const std::string& name, const glm::vec4& value);
    void SetMat3(const std::string& name, const glm::mat3& value);
    void SetMat4(const std::string& name, const glm::mat4& value);
    
private:
    unsigned int m_RendererID;
};


}




#endif /* Shader_hpp */
