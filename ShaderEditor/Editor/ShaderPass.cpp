//
//  ShaderPass.cpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 01.05.2022.
//
#include "pch.h"
#include "ShaderPass.hpp"
#include "ImGuiHelper.hpp"

namespace Editor {

ShaderPass::ShaderPass(const std::string& title)
: m_Title(title) {
    
}

ShaderPass::~ShaderPass() {
    
}

void ShaderPass::OnUpdate() {
    if(m_Shader != nullptr) {
        for(auto& object: m_ShaderPassObjects) {
            object->OnUpdate(m_Shader);
        }
    }
}


void ShaderPassInspector::Draw()  {
    if(m_Context == nullptr) return;
    
    ImGui::InputText("Title##ShaderPassTitle", &m_Context->m_Title);
}


}
