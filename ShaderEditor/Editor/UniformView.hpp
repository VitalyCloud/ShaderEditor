//
//  UniformView.hpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 01.05.2022.
//

#ifndef UniformView_hpp
#define UniformView_hpp

#include "InspectorPanel.hpp"
#include "UniformBufferContainer.hpp"

namespace Editor {

class UniformView: public Inspector {
public:
    void Draw() override;
    void SetContext(const Core::Ref<UniformBufferContainer>& context) { m_Uniforms = context; }
    
private:
    void DrawInputSettings(int index);
    void DrawUniformInput(Uniform& uniform, UniformBufferContainer::UniformData* data);
    
private:
    Core::Ref<UniformBufferContainer> m_Uniforms = nullptr;
    int m_UniformToDelete = -1;
};

}

#endif /* UniformView_hpp */
