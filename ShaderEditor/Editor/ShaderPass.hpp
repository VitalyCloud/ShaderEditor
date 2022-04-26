//
//  ShaderPass.hpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 26.04.2022.
//

#ifndef ShaderPass_hpp
#define ShaderPass_hpp

#include "VertexArrayView.hpp"

namespace Editor {

class ShaderPass {
public:
    ShaderPass(const std::string& title = "New Shader Pass");
    ~ShaderPass();
    
    void OnUpdate();
    
    std::string& GetTitle() { return m_Title; }
    
    void SetShader(const Core::Ref<OpenGL::Shader>& shader) { m_Shader = shader; }
    
    VertexArrayView& GetVA() { return m_VertexArray; }
    const Core::Ref<OpenGL::Shader> GetShader() { return m_Shader; }
private:
    std::string m_Title;
    VertexArrayView m_VertexArray;
    Core::Ref<OpenGL::Shader> m_Shader;
};


}

#endif /* ShaderPass_hpp */
