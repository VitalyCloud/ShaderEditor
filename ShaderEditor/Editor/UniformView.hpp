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
#include "Core/Core.hpp"

namespace Editor {

class UniformView {
public:
    UniformView();
    ~UniformView();
    
    void Draw();
    void SetContext(const Core::Ref<UniformBufferConteiner>& context) { m_Uniforms = context; }
private:
    void DrawUniformTypeInput(const OpenGL::ShaderDataType& type, void* data);
    void DrawInputSettings(int index);
    void DrawUniformInput(Uniform& uniform, void* data);
private:
    Core::Ref<UniformBufferConteiner> m_Uniforms = nullptr;
    
private:
    std::vector<uint32_t> m_UniformsToDelete;
};

}

#endif /* UniformView_hpp */
