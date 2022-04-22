//
//  TextEditorPanel.cpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 22.04.2022.
//

#include "TextEditorPanel.hpp"

namespace Editor {

TextEditorPanel::TextEditorPanel() { 
    AddBuffer();
}

TextEditorPanel::~TextEditorPanel() { 
    
}

void TextEditorPanel::OnEvent(Engine::Event &event) {
    if (m_SelectedBuffer != nullptr)
        m_SelectedBuffer->OnEvent(event);
}

void TextEditorPanel::AddBuffer(const std::string filepath) {
    Engine::Ref<TextBufferView> item = Engine::CreateRef<TextBufferView>();
    item->OpenFile(filepath);
    item->ShouldShow() = true;
    m_Buffers.push_back(item);
}

void TextEditorPanel::AddBuffer() {
    Engine::Ref<TextBufferView> item = Engine::CreateRef<TextBufferView>();
    item->ShouldShow() = true;
    m_Buffers.push_back(item);
}

void TextEditorPanel::CloseBuffer(Engine::Ref<TextBufferView> buffer) {
    auto it = std::find(m_Buffers.begin(), m_Buffers.end(), buffer);
    auto selected = m_Buffers.end();
    if (m_SelectedBuffer) {
        selected = std::find(m_Buffers.begin(), m_Buffers.end(), m_SelectedBuffer);
    }

    if (it != m_Buffers.end()) {
        if (selected != m_Buffers.end() && selected != it) {
            (*selected)->ShouldShow() = true;
        }
        else {
            if(it > m_Buffers.begin())
                (*(selected - 1))->ShouldShow() = true;
        }

        m_Buffers.erase(it);
    }

    it = std::find(m_BuffersToClose.begin(), m_BuffersToClose.end(), buffer);
    if (it != m_BuffersToClose.end()) {
        m_BuffersToClose.erase(it);
    }

    if (m_Buffers.empty()) {
        // Keep open at least one buffer
        m_SelectedBuffer = nullptr;
        AddBuffer();
    }
}

void TextEditorPanel::Draw(const char *title, bool *p_open) { 
    if (!ImGui::Begin(title, p_open, ImGuiWindowFlags_HorizontalScrollbar)) {
        ImGui::End();
        return;
    }
    
    if (ImGui::BeginTabBar("TabBuffers")) {
        for (int i = 0; i < m_Buffers.size(); i++) {
            auto& buffer = m_Buffers[i];
            
            ImGui::PushID(i);
            bool& shouldClose = buffer->ShouldClose();
            bool textChanged = buffer->IsTextChanged();
            bool& shouldShow = buffer->ShouldShow();
            auto tabName = buffer->GetTitle() + "##" + std::to_string(i);
            ImGuiTabItemFlags tab_flags = (textChanged ? ImGuiTabItemFlags_UnsavedDocument : 0);
            tab_flags |= (shouldShow ? ImGuiTabItemFlags_SetSelected : 0);
            if (ImGui::BeginTabItem(tabName.c_str(), nullptr, tab_flags)) {
                m_SelectedBuffer = buffer;
                if (shouldShow)
                    shouldShow = false;
                ImGui::EndTabItem();
            }
            ImGui::PopID();

            if (shouldClose) {
                if (!textChanged)
                    CloseBuffer(buffer);
                else {
                    shouldClose = false;
                    m_BuffersToClose.push_back(buffer);
                }
                    
                break;
            }
        }

        DrawConfirmationWindow();
        if (m_BuffersToClose.size() > 0 && m_ConfirmationWindowDisplayed == false)
            ImGui::OpenPopup("Save?");


        if (ImGui::TabItemButton("+", ImGuiTabItemFlags_Trailing | ImGuiTabItemFlags_NoTooltip))
            AddBuffer();

        ImGui::EndTabBar();
    }
    
    ImGui::BeginChild("BufferChild", ImVec2(0, 0), false, ImGuiWindowFlags_MenuBar);
    if (m_SelectedBuffer) {
        m_SelectedBuffer->Draw();
    }
    ImGui::EndChild();
    
    
    ImGui::End();
}

void TextEditorPanel::DrawConfirmationWindow() {
    if (ImGui::BeginPopupModal("Save?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        auto bufferToClose = m_BuffersToClose[0];
        bufferToClose->ShouldShow() = true;
        // Always center this window when appearing
        m_ConfirmationWindowDisplayed = true;
        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

        ImGui::Text("Do you want to save %s file?", bufferToClose->GetTitle().c_str());
        ImGui::Separator();

        if (ImGui::Button("Save", ImVec2(120, 0))) {
            bufferToClose->SaveFile();
            CloseBuffer(bufferToClose);
            ImGui::CloseCurrentPopup();
            m_ConfirmationWindowDisplayed = false;
        }
        ImGui::SameLine();
        if (ImGui::Button("Don't save", ImVec2(120, 0))) {
            CloseBuffer(bufferToClose);
            ImGui::CloseCurrentPopup();
            m_ConfirmationWindowDisplayed = false;
        }

        ImGui::SetItemDefaultFocus();
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(120, 0))) {
            m_BuffersToClose.clear();
            ImGui::CloseCurrentPopup();
            m_ConfirmationWindowDisplayed = false;
        }

        ImGui::EndPopup();
    }
}




}
