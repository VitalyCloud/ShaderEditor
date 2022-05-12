//
//  EditorUniforms.hpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 28.04.2022.
//

#ifndef EditorUniforms_hpp
#define EditorUniforms_hpp

#include "UniformBufferContainer.hpp"
#include "OpenGL/OpenGL.hpp"

#include <map>

namespace Editor {

class EditorUniforms {
public:
    EditorUniforms();
    ~EditorUniforms();
    
    void Upload(const Core::Ref<OpenGL::Shader>& shader);
    
    void SetResolution(const glm::vec2& resolution);
    void SetTime(float time);
    void SetViewProjection(const glm::mat4& viewProjection);
    
    static EditorUniforms& Get() { return *s_EditorUniforms; }
private:
    static EditorUniforms* s_EditorUniforms;
    UniformBufferContainer m_Uniforms;
    std::map<std::string, uint32_t> m_Locations;
};

}

#endif /* EditorUniforms_hpp */
