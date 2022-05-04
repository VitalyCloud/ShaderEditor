//
//  ShaderRenderState.hpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 04.05.2022.
//

#ifndef ShaderRenderState_hpp
#define ShaderRenderState_hpp

#include "ShaderPass.hpp"
#include "InspectorPanel.hpp"
#include "OpenGL/RenderState.hpp"

namespace Editor {

class ShaderRenderState;

class ShaderRenderStateInspector: public Inspector {
public:
    void Draw() override;
    
    void SetContext(ShaderRenderState* context) { m_Context = context; }
    const ShaderRenderState* GetContext() { return m_Context; }
private:
    ShaderRenderState* m_Context;
};

class ShaderRenderState: public ShaderPassObject {
public:
    ShaderRenderState(const std::string& title) : m_Title(title) {}
    
    void OnUpdate(const Core::Ref<OpenGL::Shader>& shader) { m_RenderState.Bind(); }
    std::string& GetTitle() { return m_Title;}
    
private:
    friend ShaderRenderStateInspector;
    std::string m_Title;
    OpenGL::RenderState m_RenderState;
};




}



#endif /* ShaderRenderState_hpp */
