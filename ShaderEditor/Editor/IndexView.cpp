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

void IndexView::Draw(std::vector<uint32_t>& context) {
    
    static int columnCount = 5;
    if(ImGui::BeginTable("IndexBufferTable", columnCount)) {
        for(int i=0; i<columnCount; i++) {
            auto title = fmt::format("{0}", i);
            ImGui::TableSetupColumn(title.c_str(), ImGuiTableColumnFlags_WidthFixed, 50);
        }
        ImGui::TableHeadersRow();
        
        for(int i=0; i<context.size(); i++) {
            if(i % columnCount == 0 || i == 0)
                ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(i % columnCount);
            ImGui::PushID(i);
            
            ImGui::PushItemWidth(ImGui::GetColumnWidth());
            ImGui::InputScalar("##indexInput", ImGuiDataType_U32, &context[i]);
            ImGui::PopItemWidth();
            if(ImGui::IsItemActive()) {
                if(ImGui::IsKeyPressed(ImGuiKey_Tab) && i == context.size()-1) {
                    context.emplace_back(0);
                }
            }
            if(ImGui::IsMouseClicked(ImGuiMouseButton_Right) && ImGui::IsItemHovered())
                ImGui::OpenPopup("IndexContext");
            if(ImGui::BeginPopup("IndexContext")) {
                if(ImGui::Selectable("Delete")) {
                    context.erase(context.begin() + i);
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
    }
}

}
