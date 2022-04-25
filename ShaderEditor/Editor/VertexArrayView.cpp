//
//  VertexArrayView.cpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 25.04.2022.
//

#include "VertexArrayView.hpp"


namespace Editor {

VertexArrayView::VertexArrayView() {
    m_VertexBuffers.push_back(Core::CreateRef<VertexBuffer>());
    m_Indicies.resize(1, 0);
}

VertexArrayView::~VertexArrayView() {
    
}

void VertexArrayView::Draw() {
    ImGui::BeginChild("VertexArrayView");
    
    for(int i=0; i<m_VertexBuffers.size(); i++) {
        auto title = fmt::format("Buffer #{0}", i);
        bool close = true;
        if(ImGui::CollapsingHeader(title.c_str(), &close)) {
            ImGui::PushID(i);
            m_VertexView.SetContext(m_VertexBuffers[i]);
            m_VertexView.Draw();
            ImGui::PopID();
        }
        
        if(!close) {
            m_VertexBuffers.erase(m_VertexBuffers.begin() + i);
        }
    }
    
    if(ImGui::Button("Add Buffer")) {
        m_VertexBuffers.push_back(Core::CreateRef<VertexBuffer>());
    }
    
    ImGui::Separator();
    
    if(ImGui::CollapsingHeader("Index Buffer")) {
        m_IndexView.Draw(m_Indicies);
    }
    
    ImGui::EndChild();
}

}
