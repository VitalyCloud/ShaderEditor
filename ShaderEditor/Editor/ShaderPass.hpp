//
//  ShaderPass.hpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 01.05.2022.
//

#ifndef ShaderPass_hpp
#define ShaderPass_hpp

#include "OpenGL/Shader.hpp"
#include "InspectorPanel.hpp"

namespace Editor {

class ShaderPassObject {
public:
    virtual void OnUpdate(const Core::Ref<OpenGL::Shader>& shader) = 0;
    virtual std::string& GetTitle() = 0;
};

class ShaderPassInspector;

class ShaderPass {
public:
    ShaderPass(const std::string& title = "ShaderPass");
    ~ShaderPass();
    
    void OnUpdate();
    
    const std::string& GetTitle() const { return m_Title; }
    std::vector<Core::Ref<ShaderPassObject>>& GetShaderPassObjects() { return m_ShaderPassObjects; }
    const Core::Ref<OpenGL::Shader>& GetShader() { return m_Shader; }
    
    void SetVertexPath(const Core::Ref<Core::Utils::File>& vertexPath);
    void SetFragmnetPath(const Core::Ref<Core::Utils::File>& fragmentPath);
    void SetShaderPath(const Core::Ref<Core::Utils::File>& vertexPath,
                       const Core::Ref<Core::Utils::File>& fragmentPath);
    void SetAutoCompile(bool value) { m_AutoCompile = value; }
    bool IsAutoCompile() { return m_AutoCompile; }
private:
    void UpdateShader();
    
private:
    friend ShaderPassInspector;
    std::string m_Title;
    std::vector<Core::Ref<ShaderPassObject>> m_ShaderPassObjects;
    
    Core::Ref<OpenGL::Shader> m_Shader = nullptr;
    Core::Ref<Core::Utils::File> m_VertexPath = nullptr;
    Core::Ref<Core::Utils::File> m_FragmnetPath = nullptr;
    
    bool m_AutoCompile = false;
};


class ShaderPassInspector: public Inspector {
public:
    void Draw() override;
    
    void SetContext(const Core::Ref<ShaderPass> context) { m_Context = context; }
    const Core::Ref<ShaderPass>& GetContext() { return m_Context; }
    
private:
    Core::Ref<ShaderPass> m_Context = nullptr;
};


}

#endif /* ShaderPass_hpp */
