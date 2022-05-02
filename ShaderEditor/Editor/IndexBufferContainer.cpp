//
//  IndexBufferContainer.cpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 01.05.2022.
//
#include "pch.h"
#include "IndexBufferContainer.hpp"

#include "imgui.h"

namespace Editor {

IndexBufferContainer::IndexBufferContainer() {
    m_IndexBuffer = Core::CreateRef<OpenGL::IndexBuffer>(1);
    
    // Default value
    m_Data.push_back(0);
    m_State.Set(IndexBufferState::DataChanged);
    UpdateIndexBufferIfNeeded();
}

IndexBufferContainer::~IndexBufferContainer() {
    
}

void IndexBufferContainer::SetData(const std::vector<uint32_t>& data) {
    m_Data = data;
    m_State.Set(IndexBufferState::SizeChanged);
}

void IndexBufferContainer::UpdateIndexBufferIfNeeded() {
    if(m_State.CheckIf(IndexBufferState::SizeChanged)) {
        EN_INFO("Handle IB size change");
        m_IndexBuffer->Resize((uint32_t)m_Data.size());
        m_IndexBuffer->SetData(m_Data.data(), (uint32_t)m_Data.size());
        m_State.Reset();
        return;
    }
    
    if(m_State.CheckIf(IndexBufferState::DataChanged)) {
        EN_INFO("Handle IB data change");
        m_IndexBuffer->SetData(m_Data.data(), (uint32_t)m_Data.size());
        m_State.Reset();
        return;
    }
    
}

void IndexView::Draw() {
    if(m_Context == nullptr) return;
    std::vector<uint32_t>& context = m_Context->m_Data;
    
    const static int columnWidth = 50;
    float avalWidth = ImGui::GetWindowWidth();
    int columnCount = (int)(avalWidth / columnWidth) - 2;
    columnCount = columnCount > 1 ? columnCount : 1;
//    m_Changed = false;
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
            if(changed)
                m_Context->GetState().Set(IndexBufferState::DataChanged);
            
            ImGui::PopItemWidth();
            if(ImGui::IsItemActive()) {
                if(!ImGui::IsKeyDown(ImGuiKey_LeftShift) && ImGui::IsKeyPressed(ImGuiKey_Tab) && i == context.size()-1) {
                    context.emplace_back(0);
                    m_Context->GetState().Set(IndexBufferState::SizeChanged);
                }
                if(ImGui::IsKeyDown(ImGuiKey_LeftShift) && ImGui::IsKeyPressed(ImGuiKey_Backspace) && context.size() > 1) {
                    context.erase(context.begin() + i);
                    needFocusIndex = i-1;
                    m_Context->GetState().Set(IndexBufferState::SizeChanged);
                }
            }
            if(ImGui::IsMouseClicked(ImGuiMouseButton_Right) && ImGui::IsItemHovered())
                ImGui::OpenPopup("IndexContext");
            if(ImGui::BeginPopup("IndexContext")) {
                if(ImGui::Selectable("Delete")) {
                    context.erase(context.begin() + i);
                    m_Context->GetState().Set(IndexBufferState::SizeChanged);
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
        m_Context->GetState().Set(IndexBufferState::SizeChanged);
    }
    
    ImGui::SameLine();
}


}
