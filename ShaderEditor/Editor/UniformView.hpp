//
//  UniformView.hpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 23.04.2022.
//

#ifndef UniformView_hpp
#define UniformView_hpp

#include "OpenGL/Buffer.hpp"
#include "UniformBuffer.hpp"

namespace Editor {

class UniformView {
public:
    UniformView();
    ~UniformView();
    
    void Draw();
private:
    void DrawUniformTypeInput(const OpenGL::ShaderDataType& type, void* data);
    void DrawInputSettings(int index);
    void DrawUniformInput(Uniform& uniform, void* data);
    void DrawInputFloat(Uniform& uniform, void* data);
    void DrawInputInt(Uniform& uniform, void* data);
private:
    UniformBuffer m_Uniforms;
    
private:
    std::vector<uint32_t> m_UniformsToDelete;
};

}

#endif /* UniformView_hpp */
