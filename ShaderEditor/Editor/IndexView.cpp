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
    
}

IndexView::~IndexView() {
    
}

void IndexView::Draw() {
    
    if(m_Context == nullptr) return;
    std::vector<uint32_t>& context = *m_Context;
    
    const static int columnWidth = 50;
    float avalWidth = ImGui::GetWindowWidth();
    int columnCount = (int)(avalWidth / columnWidth) - 2;
    columnCount = columnCount > 1 ? columnCount : 1;
    m_Changed = false;
    if(ImGui::BeginTable("IndexBufferTable", columnCount)) {
        for(int i=0; i<columnCount; i++) {
            auto title = fmt::format("{0}", i);
            ImGui::TableSetupColumn(title.c_str(), ImGuiTableColumnFlags_WidthFixed, columnWidth);
        }
        ImGui::TableHeadersRow();
        
        static int needFocusIndex = -1;
        for(int i=0; i<context.size(); i++) {
            if(i % columnCount == 0 || i == 0)
                ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(i % columnCount);
            ImGui::PushID(i);
            
            ImGui::PushItemWidth(ImGui::GetColumnWidth());
            if(needFocusIndex == i) {
                ImGui::SetKeyboardFocusHere();
                needFocusIndex = -1;
            }
            bool changed = ImGui::InputScalar("##indexInput", ImGuiDataType_U32, &context[i]);
            if(changed && m_AutoChange)
                m_Changed = changed;
            ImGui::PopItemWidth();
            if(ImGui::IsItemActive()) {
                if(!ImGui::IsKeyDown(ImGuiKey_LeftShift) && ImGui::IsKeyPressed(ImGuiKey_Tab) && i == context.size()-1) {
                    context.emplace_back(0);
                    if(m_AutoChange) m_Changed = true;
                }
                if(ImGui::IsKeyDown(ImGuiKey_LeftShift) && ImGui::IsKeyPressed(ImGuiKey_Backspace) && context.size() > 1) {
                    context.erase(context.begin() + i);
                    needFocusIndex = i-1;
                    if(m_AutoChange) m_Changed = true;
                }
            }
            if(ImGui::IsMouseClicked(ImGuiMouseButton_Right) && ImGui::IsItemHovered())
                ImGui::OpenPopup("IndexContext");
            if(ImGui::BeginPopup("IndexContext")) {
                if(ImGui::Selectable("Delete")) {
                    context.erase(context.begin() + i);
                    if(m_AutoChange) m_Changed = true;
                }
                ImGui::EndPopup();
            }
            ImGui::PopID();
        }
        ImGui::EndTable();
    }
    if(ImGui::Button("Clear")) {
        context.clear();
        context.emplace_back(0);
        if(m_AutoChange) m_Changed = true;
    }
    
    ImGui::SameLine();
    ImGui::Checkbox("Auto change", &m_AutoChange);
    if(!m_AutoChange) {
        ImGui::SameLine();
        if(ImGui::Button("Apply")) {
            m_Changed = true;
        }
    }
        
}

}
