//
//  IndexView.cpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 25.04.2022.
//

#include "IndexView.hpp"

#include "imgui.h"

namespace Editor {

IndexView::IndexView() {
    m_Indicies.resize(1);
}

IndexView::~IndexView() {
    
}

void IndexView::Draw() {
    ImGui::BeginChild("IndexView");
    
    static int columnCount = 8;
    if(ImGui::BeginTable("IndexBufferTable", columnCount)) {
        for(int i=0; i<columnCount; i++) {
            auto title = fmt::format("{0}", i);
            ImGui::TableSetupColumn(title.c_str(), ImGuiTableColumnFlags_WidthFixed, 50);
        }
        ImGui::TableHeadersRow();
        
        for(int i=0; i<m_Indicies.size(); i++) {
            if(i % columnCount == 0 || i == 0)
                ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(i % columnCount);
            ImGui::PushID(i);
            
            ImGui::PushItemWidth(ImGui::GetColumnWidth());
            ImGui::InputScalar("##indexInput", ImGuiDataType_U32, &m_Indicies[i]);
            ImGui::PopItemWidth();
            if(ImGui::IsItemActive()) {
                if(ImGui::IsKeyPressed(ImGuiKey_Tab) && i == m_Indicies.size()-1) {
                    m_Indicies.emplace_back();
                }
            }
            if(ImGui::IsMouseClicked(ImGuiMouseButton_Right) && ImGui::IsItemHovered())
                ImGui::OpenPopup("IndexContext");
            if(ImGui::BeginPopup("IndexContext")) {
                if(ImGui::Selectable("Delete")) {
                    m_Indicies.erase(m_Indicies.begin() + i);
                }
                ImGui::EndPopup();
            }
            ImGui::PopID();
        }
        ImGui::EndTable();
    }
    if(ImGui::Button("Clear")) {
        m_Indicies.clear();
        m_Indicies.emplace_back();
    }
    
    
    ImGui::EndChild();
}

}
