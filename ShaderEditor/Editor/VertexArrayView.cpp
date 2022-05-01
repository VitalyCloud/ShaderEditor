//
//  VertexArrayView.cpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 01.05.2022.
//

#include "pch.h"
#include "VertexArrayView.hpp"

namespace Editor {

void VertexArrayView::Draw() {
    
    for(int i=0; i<m_Context->GetVertexConteiners().size(); i++) {
        ImGui::PushID(i);
        auto title = fmt::format("Buffer #{0}", i);
        bool close = true;
        bool open = false;
        if(m_Context->GetVertexConteiners().size() == 1)
            open = ImGui::CollapsingHeader(title.c_str());
        else
            open = ImGui::CollapsingHeader(title.c_str(), &close);
        
        if(ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
            ImGui::OpenPopup("BufferContext");
        }
        
        if(ImGui::BeginPopup("BufferContext")) {
            if(ImGui::Selectable("Add Buffer")) {
                m_Context->AddVertexBuffer();
            }
            ImGui::EndPopup();
        }
        
        auto& vertexContainer = m_Context->GetVertexConteiners()[i];
        
        if(m_AutoChange) {
            bool isLayoutChanged = vertexContainer->IsLayoutChanged();
            vertexContainer->UpdateVertexBufferIfNeeded();
            if(isLayoutChanged)
                m_Context->InvalidateVertexArray();
        }
            
        
        if(open) {
            ImGui::PushID(i);
            m_VertexView.SetContext(vertexContainer);
            m_VertexView.Draw();
            ImGui::PopID();
        }
        
        if(!close && m_Context->GetVertexConteiners().size() > 1) {
            m_VertexContainerToDelete = i;
        }
        
        
        ImGui::Checkbox("Auto change", &m_AutoChange);
        if(!m_AutoChange) {
            ImGui::SameLine();
            if(ImGui::Button("Apply")) {
                bool layoutChanged = vertexContainer->IsLayoutChanged();
                vertexContainer->UpdateVertexBufferIfNeeded();
                if(layoutChanged)
                    m_Context->InvalidateVertexArray();
            }
        }
        ImGui::PopID();
        
    }
   
    
    
    if(ImGui::CollapsingHeader("Index Buffer")) {
        m_IndexView.SetContext(m_Context->GetIndexConteiner());
        m_IndexView.Draw();
//        if(m_IndexView.IsChanged()) {
//            m_Context->InvalidateVertexArray();
//        }
    }
    
    
    if(m_VertexContainerToDelete > -1)
        m_Context->RemoveVertexBuffer(m_VertexContainerToDelete);
    m_VertexContainerToDelete = -1;
}

}
