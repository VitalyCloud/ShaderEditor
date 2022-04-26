//
//  Shader.cpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 21.04.2022.
//
#include "Core/pch.h"
#include "Shader.hpp"
#include "glad/glad.h"

#include "glm/gtc/type_ptr.hpp"


namespace OpenGL {

Shader::Shader(const std::string &vertexSrc, const std::string &fragmentSrc) { 
    // Vertex Shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const GLchar* source = vertexSrc.c_str();
    glShaderSource(vertexShader, 1, &source, 0);
    glCompileShader(vertexShader);
    
    GLint isCompiled = 0;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
    if(isCompiled == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);
        std::vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);
        glDeleteShader(vertexShader);
        
        EN_CORE_ERROR("Vertex Shader Compilation Failed: {0}", infoLog.data());
        return;
    }
    
    // Fragment Shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    source = fragmentSrc.c_str();
    glShaderSource(fragmentShader, 1, &source, 0);
    glCompileShader(fragmentShader);
    
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
    if(isCompiled == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);
        std::vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);
        
        glDeleteShader(fragmentShader);
        glDeleteShader(vertexShader);
    
        EN_CORE_ERROR("Fragment Shader Compilation Failed: {0}", infoLog.data());
        return;
    }
    
    // Linking
    
    m_RendererID = glCreateProgram();
    GLint program = m_RendererID;
    
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    
    GLint isLinked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
    if(isLinked == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);
        
        glDeleteProgram(program);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        
        EN_CORE_ERROR("Program Linking Failed: {0}", infoLog.data());
        return;
    }
    
    glDetachShader(program, vertexShader);
    glDetachShader(program, fragmentShader);
}

Shader::~Shader() { 
    glDeleteProgram(m_RendererID);
}

void Shader::Bind() { 
    glUseProgram(m_RendererID);
}

void Shader::Unbind() { 
    glUseProgram(0);
}

void Shader::SetInt(const std::string &name, int value) { 
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform1i(location, value);
}

void Shader::SetIntArray(const std::string &name, int *values, uint32_t count) { 
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform1iv(location, count, values);
}

void Shader::SetFloatArray(const std::string& name, float* values, uint32_t count) {
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform1fv(location, count, values);
}

void Shader::SetFloat(const std::string &name, float value) { 
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform1f(location, value);
}

void Shader::SetFloat2(const std::string &name, const glm::vec2 &value) { 
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform2f(location, value.x, value.y);
}

void Shader::SetFloat3(const std::string &name, const glm::vec3 &value) { 
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform3f(location, value.x, value.y, value.z);
}

void Shader::SetFloat4(const std::string &name, const glm::vec4 &value) { 
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform4f(location, value.x, value.y, value.z, value.w);
}

void Shader::SetMat3(const std::string &name, const glm::mat3 &value) { 
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::SetMat3v(const std::string& name, const float* values) {
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniformMatrix3fv(location, 1, GL_FALSE, values);
}

void Shader::SetMat4(const std::string &name, const glm::mat4 &value) { 
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::SetMat4v(const std::string& name, const float* values) {
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, values);
}

}
