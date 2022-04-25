//
//  Pipeline.hpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 25.04.2022.
//

#ifndef Pipeline_hpp
#define Pipeline_hpp

#include <stdio.h>

#include "Core/Core.hpp"
#include "OpenGL/OpenGL.hpp"

#include "VertexView.hpp"
#include "IndexView.hpp"

namespace Editor {

class VertexArray {
public:
    VertexArray();
    ~VertexArray();
    
    void OnUpdate();
    
    void InvalidateVertexArray();
    
    void Draw();
    
    const Core::Ref<OpenGL::VertexArray>& GetOpenGLVA() { return m_VertexArrayInternal; }
    uint32_t GetVertexCount() { return m_VertexBuffers[0]->VertexCount(); }
    
private:
    void AddVertexBuffer();
    void RemoveVertexBuffer(int index);
    void InvalidateChanges(int index);

private:
    VertexView m_VertexView;
    IndexView m_IndexView;
    
    std::vector<Core::Ref<VertexBuffer>> m_VertexBuffers;
    bool m_VertexBufferChanged = false;
    
    std::vector<uint32_t> m_Indicies;
    
    Core::Ref<OpenGL::VertexArray> m_VertexArrayInternal;
};

class ShaderPass {
public:
    ShaderPass(const std::string& title = "New Shader Pass");
    ~ShaderPass();
    
    void OnUpdate();
    
    std::string& GetTitle() { return m_Title; }
    
    VertexArray& GetVA() { return m_VertexArray; }
    
private:
    std::string m_Title;
    VertexArray m_VertexArray;
    Core::Ref<OpenGL::Shader> m_Shader;
};


class Pipeline {
public:
    Pipeline();
    ~Pipeline();
    
    void OnUpdate();
    void Draw(const char* title, bool* p_open = nullptr);
    
    void DrawInspector();
    
    
    
private:
    std::vector<Core::Ref<ShaderPass>> m_ShaderPassses;
    Core::Ref<ShaderPass> m_SelectedShaderPasss;
};


}

#endif /* Pipeline_hpp */
