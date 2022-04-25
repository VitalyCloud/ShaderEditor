//
//  UniformView.hpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 23.04.2022.
//

#ifndef UniformView_hpp
#define UniformView_hpp

#include "OpenGL/Buffer.hpp"
#include "UniformBufferConteiner.hpp"

namespace Editor {

class UniformPanel {
public:
    UniformPanel();
    ~UniformPanel();
    
    void Draw(const char* title, bool* p_open = nullptr);
private:
    void DrawUniformTypeInput(const OpenGL::ShaderDataType& type, void* data);
    void DrawInputSettings(int index);
    void DrawUniformInput(Uniform& uniform, void* data);
private:
    UniformBufferConteiner m_Uniforms;
    
private:
    std::vector<uint32_t> m_UniformsToDelete;
};

}

#endif /* UniformView_hpp */
