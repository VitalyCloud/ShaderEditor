//
//  EditorUniforms.cpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 28.04.2022.
//

#include "Core/Core.hpp"
#include "EditorUniforms.hpp"

#include "glm/gtc/type_ptr.hpp"

namespace Editor {

static const std::string UniformResolution = "u_Resolution";
static const std::string UniformTime = "u_Time";
static const std::string ViewProjection = "u_ViewProjection";

EditorUniforms* EditorUniforms::s_EditorUniforms = nullptr;

EditorUniforms::EditorUniforms() {
    EN_ASSERT(s_EditorUniforms == nullptr, "EditorUniforms is already created");
    s_EditorUniforms = this;
    
    m_Locations[UniformResolution] = 0;
    m_Uniforms.PushUniform(OpenGL::ShaderDataType::Float2, UniformResolution);
    
    m_Locations[UniformTime] = 1;
    m_Uniforms.PushUniform(OpenGL::ShaderDataType::Float, UniformTime);
    
    m_Locations[ViewProjection] = 2;
    m_Uniforms.PushUniform(OpenGL::ShaderDataType::Mat4, ViewProjection);
}

void EditorUniforms::SetResolution(const glm::vec2& resolution) {
    m_Uniforms.SetUniformData(m_Locations[UniformResolution], glm::value_ptr(resolution));
}

void EditorUniforms::SetTime(float time) {
    m_Uniforms.SetUniformData(m_Locations[UniformTime], &time);
}

void EditorUniforms::SetViewProjection(const glm::mat4& viewProjection) {
    m_Uniforms.SetUniformData(m_Locations[ViewProjection], glm::value_ptr(viewProjection));
}

void EditorUniforms::Upload(const Core::Ref<OpenGL::Shader>& shader) {
    m_Uniforms.UploadUniforms(shader);
}

EditorUniforms::~EditorUniforms() {
    
}

}
