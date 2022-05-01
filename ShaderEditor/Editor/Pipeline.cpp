//
//  Pipeline.cpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 01.05.2022.
//
#include "pch.h"
#include "Pipeline.hpp"


namespace Editor {

Pipeline::Pipeline() {
    
}

Pipeline::~Pipeline() {
    
}

void Pipeline::OnUpdate() {
    for(auto& shaderPass: m_ShaderPasses) {
        shaderPass->OnUpdate();
    }
}

}
