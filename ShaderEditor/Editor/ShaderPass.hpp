//
//  ShaderPass.hpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 26.04.2022.
//

#ifndef ShaderPass_hpp
#define ShaderPass_hpp

#include "InspectorPanel.hpp"
#include "Mesh.hpp"
#include "UniformBufferConteiner.hpp"
#include "UniformView.hpp"

namespace Editor {

class ShaderPassInspector;

class ShaderPass {
public:
    ShaderPass(const std::string& title = "New Shader Pass");
    ~ShaderPass();
    
    void OnUpdate();
    
    void SetShader(const Core::Ref<OpenGL::Shader>& shader) { m_Shader = shader; }
    void SetShaderPath(const Core::Ref<Core::Utils::File>& vertexPath,
                       const Core::Ref<Core::Utils::File>& fragmentPath);
    void SetVertexPath(const Core::Ref<Core::Utils::File>& vertexPath);
    void SetFragmnetPath(const Core::Ref<Core::Utils::File>& fragmentPath);
    
    std::vector<Core::Ref<Mesh>>& GetMeshes() { return m_Meshes; }
    std::string& GetTitle() { return m_Title; }
    const Core::Ref<OpenGL::Shader> GetShader() { return m_Shader; }
    
private:
    void UpdateShader();
    
private:
    friend class ShaderPassInspector;
    std::string m_Title;
    std::vector<Core::Ref<Mesh>> m_Meshes;
    Core::Ref<UniformBufferConteiner> m_Uniforms;
    
    Core::Ref<OpenGL::Shader> m_Shader;
    Core::Ref<Core::Utils::File> m_VertexPath;
    Core::Ref<Core::Utils::File> m_FragmnetPath;
};

class ShaderPassInspector: public InspectorItem {
public:
    void Draw() override;
    void SetContext(const Core::Ref<ShaderPass>& context) { m_Context = context; }
    const Core::Ref<ShaderPass>& GetContext() { return m_Context; }
private:
    Core::Ref<ShaderPass> m_Context = nullptr;
    UniformView m_UniformView;
    
    enum class ActionPopupSelector {
        Vertex, Fragment
    };
    
    ActionPopupSelector m_ActionPopupSelector = ActionPopupSelector::Vertex;
};


}

#endif /* ShaderPass_hpp */
