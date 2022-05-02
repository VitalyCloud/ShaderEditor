//
//  Pipeline.cpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 01.05.2022.
//
#include "pch.h"
#include "Pipeline.hpp"

#include "Mesh.hpp"


namespace Editor {

const std::string vertex = "/Users/vitalycloud/Desktop/ShaderEditor 2/Resources/BasicShader/Basic.vert";
const std::string fragment = "/Users/vitalycloud/Desktop/ShaderEditor 2/Resources/BasicShader/Basic.frag";

Pipeline::Pipeline() {
    auto shaderPass = Core::CreateRef<ShaderPass>("MyShader");
    
    auto vertexPath = Core::Utils::FileWatcher::Get().LoadFile(vertex);
    auto fragmentPath = Core::Utils::FileWatcher::Get().LoadFile(fragment);
    shaderPass->SetShaderPath(vertexPath, fragmentPath);
    
    shaderPass->GetShaderPassObjects().push_back(Core::CreateRef<Mesh>(DefaultMesh::Triangle));
    
    m_ShaderPasses.push_back(shaderPass);
}

Pipeline::~Pipeline() {
    
}

void Pipeline::OnUpdate() {
    for(auto& shaderPass: m_ShaderPasses) {
        shaderPass->OnUpdate();
    }
}

}
