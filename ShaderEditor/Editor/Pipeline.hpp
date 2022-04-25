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
    
    void Draw();

private:
    VertexView m_VertexView;
    IndexView m_IndexView;
    
    std::vector<Core::Ref<VertexBuffer>> m_VertexBuffers;
    std::vector<uint32_t> m_Indicies;
};

class ShaderPass {
public:
    ShaderPass(const std::string& title = "New Shader Pass") : m_Title(title) {}
    ~ShaderPass() {}
    
    std::string& GetTitle() { return m_Title; }
    
    VertexArray& GetVA() { return m_VertexArray; }
    
private:
    std::string m_Title;
    VertexArray m_VertexArray;
};


class Pipeline {
public:
    Pipeline();
    ~Pipeline();
    
    void Draw(const char* title, bool* p_open = nullptr);
    
    void DrawInspector();
    
    
    
private:
    std::vector<Core::Ref<ShaderPass>> m_ShaderPassses;
    Core::Ref<ShaderPass> m_SelectedShaderPasss;
};


}

#endif /* Pipeline_hpp */
