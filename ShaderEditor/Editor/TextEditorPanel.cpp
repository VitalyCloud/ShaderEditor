//
//  TextEditorPanel.cpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 05.05.2022.
//
#include "pch.h"
#include "TextEditorPanel.hpp"

#include "Core/ImGui/Menlo-Regular.embed"

namespace Editor {


TextEditorPanel::TextEditorPanel() {
    m_Buffers.emplace_back();
    
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImFontConfig fontConfig;
    fontConfig.FontDataOwnedByAtlas = false;
    m_Font = io.Fonts->AddFontFromMemoryTTF((void*)g_MenloRegular, sizeof(g_MenloRegular), 20, &fontConfig);
}

TextEditorPanel::~TextEditorPanel() {
    
}

int TextEditorPanel::GetIndexForFile(const Core::Ref<Core::Utils::File>& file) {
    for(int i=0; i<m_Buffers.size(); i++) {
        auto bufferFile = m_Buffers[i].File;
        if(bufferFile != nullptr && bufferFile->GetPath() == file->GetPath())
            return i;
    }
    return -1;
}

void TextEditorPanel::OpenFile(const Core::Ref<Core::Utils::File>& file) {
    
    Core::Ref<Core::Utils::File> fileToOpen = file;
    if(fileToOpen == nullptr) {
        auto path = Core::Utils::FileDialogs::OpenFile("");
        if(!path.empty())
            fileToOpen = Core::Utils::FileWatcher::Get().LoadFile(path);
        else
            return;
    }
    
    int index = GetIndexForFile(fileToOpen);
    if(index > -1) {
        m_Buffers[index].ShouldShow = true;
    } else {
        m_Buffers.emplace_back(fileToOpen);
    }
}


void TextEditorPanel::OnEvent(Core::Event& event) {
    Core::EventDispatcher dispatcher(event);
    dispatcher.Dispatch<Core::KeyPressedEvent>(BIND_EVENT_FN(&TextEditorPanel::OnKeyPressed));
}

bool TextEditorPanel::OnKeyPressed(Core::KeyPressedEvent& event) {
    if (event.GetRepeatCount() > 0)
        return false;
    bool control = Core::Input::IsKeyPressed(Core::Key::LeftControl) ||
        Core::Input::IsKeyPressed(Core::Key::RightControl);
    
    switch (event.GetKeyCode()) {
    case Core::Key::S:
        if (control && m_SelectedBuffer > -1)
            m_Buffers[m_SelectedBuffer].SaveFile();
        break;
    case Core::Key::O:
        if (control) {
            OpenFile();
        }
    default:
        break;
    }
    
    return true;
}

void TextEditorPanel::Draw(const char* title, bool* p_open) {
    if (!ImGui::Begin(title, p_open, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_MenuBar)) {
        ImGui::End();
        return;
    }
    
    if(m_Buffers.size() > 0) {
        if(ImGui::BeginTabBar("Buffers##TabBarBuffers")) {
            for(int i=0; i < m_Buffers.size(); i++) {
                ImGui::PushID(i);
             
                auto tabName = m_Buffers[i].GetTitle();
                ImGuiTabItemFlags tabFlags = (m_Buffers[i].TextChanged ? ImGuiTabItemFlags_UnsavedDocument : 0);
                tabFlags |= (m_Buffers[i].ShouldShow ? ImGuiTabItemFlags_SetSelected : 0);
                
                bool keepOpen = true;
                if(ImGui::BeginTabItem(tabName.c_str(), &keepOpen, tabFlags)) {
                    m_SelectedBuffer = i;
                    if(m_Buffers[i].ShouldShow)
                        m_Buffers[i].ShouldShow = false;
                    ImGui::EndTabItem();
                }
                
                if(!keepOpen || m_Buffers[i].ShouldClose) {
                    m_Buffers.erase(m_Buffers.begin() + i);
                    m_SelectedBuffer = -1;
                }
                
                if(!p_open)
                    EN_INFO("Close");
                
                ImGui::PopID();
            }
            ImGui::EndTabBar();
        }
    }
    
    DrawMenuBar();
    
    if(m_SelectedBuffer > -1) {
        auto& buffer = m_Buffers[m_SelectedBuffer].Buffer;
        auto title = m_Buffers[m_SelectedBuffer].GetTitle();
        
        auto cpos = buffer.GetCursorPosition();
        ImGui::SetWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);
        
        ImGui::Text("%6d/%-6d %6d lines  | %s | %s | %s | %s", cpos.mLine + 1, cpos.mColumn + 1, buffer.GetTotalLines(),
                    buffer.IsOverwrite() ? "Ovr" : "Ins",
                    buffer.CanUndo() ? "*" : " ",
                    buffer.GetLanguageDefinition().mName.c_str(), title.c_str());
        
        ImGui::PushFont(m_Font);
        m_Buffers[m_SelectedBuffer].Draw();
        ImGui::PopFont();
    }
    
    ImGui::End();
}

void TextEditorPanel::DrawMenuBar() {
    if(ImGui::BeginMenuBar()) {
        
        
        if(ImGui::BeginMenu("File")) {
            
            if(ImGui::MenuItem("Open"))
                OpenFile();
            
            if(m_SelectedBuffer > -1) {
                if(ImGui::MenuItem("Save"))
                    m_Buffers[m_SelectedBuffer].SaveFile();
                if(ImGui::MenuItem("Close"))
                    m_Buffers[m_SelectedBuffer].ShouldClose = true;
            }
            
            ImGui::EndMenu();
        }
        
        if(m_SelectedBuffer > -1) {
            if(ImGui::BeginMenu("Edit")) {
                auto& buffer = m_Buffers[m_SelectedBuffer].Buffer;
                bool ro = buffer.IsReadOnly();
                
                if (ImGui::MenuItem("Undo", "ALT-Backspace", nullptr, !ro && buffer.CanUndo()))
                    buffer.Undo();
                if (ImGui::MenuItem("Redo", "Ctrl-Y", nullptr, !ro && buffer.CanRedo()))
                    buffer.Redo();
                
                ImGui::Separator();
                
                if (ImGui::MenuItem("Copy", "Ctrl-C", nullptr, buffer.HasSelection()))
                    buffer.Copy();
                if (ImGui::MenuItem("Cut", "Ctrl-X", nullptr, !ro && buffer.HasSelection()))
                    buffer.Cut();
                if (ImGui::MenuItem("Delete", "Del", nullptr, !ro && buffer.HasSelection()))
                    buffer.Delete();
                if (ImGui::MenuItem("Paste", "Ctrl-V", nullptr, !ro && ImGui::GetClipboardText() != nullptr))
                    buffer.Paste();
                
                ImGui::Separator();
                
                if (ImGui::MenuItem("Select all", nullptr, nullptr))
                    buffer.SetSelection(TextEditor::Coordinates(), TextEditor::Coordinates(buffer.GetTotalLines(), 0));
            
                ImGui::EndMenu();
            }
        }
        
        ImGui::EndMenuBar();
    }
}

}
