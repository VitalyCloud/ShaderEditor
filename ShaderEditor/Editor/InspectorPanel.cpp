//
//  InspectorPanel.cpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 27.04.2022.
//

#include "InspectorPanel.hpp"

namespace Editor {

InspectorPanel* InspectorPanel::s_InspectorPanel;

InspectorPanel::InspectorPanel() {
    EN_ASSERT(s_InspectorPanel == nullptr, "Inspector Panel is already created");
    s_InspectorPanel = this;
}

InspectorPanel::~InspectorPanel() {
    
}

void InspectorPanel::Draw(const char* title, bool* p_open ) {
    if (!ImGui::Begin(title, p_open)) {
        ImGui::End();
        return;
    }
    
    if(m_Context != nullptr)
        m_Context->Draw();
    
    ImGui::End();
}

}
