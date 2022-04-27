//
//  Mesh.cpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 27.04.2022.
//

#include "Core/pch.h"
#include "Mesh.hpp"

#include "ImGuiHelper.h"

namespace Editor {

void MeshInspector::Draw() {
    if(m_Context == nullptr)
        return;
    
    ImGui::InputText("Title##MeshTitle", &m_Context->m_Title);
}

}
