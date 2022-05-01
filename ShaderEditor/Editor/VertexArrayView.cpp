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
        
        if(m_AutoChange) {
            m_Context->GetVertexConteiners()[i]->UpdateVertexBufferIfNeeded();
            if(m_LayoutChanged)
                m_Context->InvalidateVertexArray();
            m_Context->GetVertexConteiners()[i]->GetState().Reset();
            m_LayoutChanged = false;
        }
            
        
        if(open) {
            ImGui::PushID(i);
            m_VertexView.SetContext(m_Context->GetVertexConteiners()[i]);
            m_VertexView.Draw();
            ImGui::PopID();
        }
        
        if(!close && m_Context->GetVertexConteiners().size() > 1) {
            m_Context->RemoveVertexBuffer(i);
        }
        
        bool layoutChanged = m_Context->GetVertexConteiners()[i]->GetState().CheckIf(VertexBufferState::LayoutChanged);
        
        if(layoutChanged)
            m_LayoutChanged = true;
        
        ImGui::Checkbox("Auto change", &m_AutoChange);
        if(!m_AutoChange) {
            ImGui::SameLine();
            if(ImGui::Button("Apply")) {
                
                m_Context->GetVertexConteiners()[i]->UpdateVertexBufferIfNeeded();
                if(m_LayoutChanged)
                    m_Context->InvalidateVertexArray();
                
                m_Context->GetVertexConteiners()[i]->GetState().Reset();
                m_LayoutChanged = false;
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
    
}

}
