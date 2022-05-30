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

TextEditorPanel* TextEditorPanel::s_TextEditorPanel = nullptr;

TextEditorPanel::TextEditorPanel() {
    EN_ASSERT(s_TextEditorPanel == nullptr, "Text Editor Panel is already created");
    s_TextEditorPanel = this;

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImFontConfig fontConfig;
    fontConfig.FontDataOwnedByAtlas = false;
    m_Font = io.Fonts->AddFontFromMemoryTTF((void*)g_MenloRegular, sizeof(g_MenloRegular), 16, &fontConfig);
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

void TextEditorPanel::CloseBuffer(int index) {
    EN_ASSERT(index < m_Buffers.size(), "Index is out of range");
    m_Buffers.erase(m_Buffers.begin() + index);
    if(index < m_Buffers.size()) {
        m_SelectedBuffer = index;
        m_Buffers[index].ShouldShow = true;
    } else {
        m_SelectedBuffer = -1;
    }
}

void TextEditorPanel::AddBuffer() {
    m_Buffers.emplace_back();
    m_Buffers[m_Buffers.size() - 1].ShouldShow = true;
}

void TextEditorPanel::Draw(const char* title, bool* p_open) {
    if (!ImGui::Begin(title, p_open, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_MenuBar)) {
        ImGui::End();
        return;
    }
    
   
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
                if(!m_Buffers[i].TextChanged)
                    m_BuffersToClose.push_back(i);
                else {
                    m_Buffers[i].ShouldClose = false;
                    m_BuffersToCloseWithConfiramtion.push_back(i);
                }
            }
            
            ImGui::PopID();
        }
        if (ImGui::TabItemButton("+", ImGuiTabItemFlags_Trailing | ImGuiTabItemFlags_NoTooltip))
            AddBuffer();
        ImGui::EndTabBar();
    }
    
    
    DrawMenuBar();
    DrawCloseConfirmationWindow();
    
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
    
    for(auto i : m_BuffersToClose) {
        CloseBuffer(i);
    }
    m_BuffersToClose.clear();
    
    ImGui::End();
}

void TextEditorPanel::DrawMenuBar() {
    if(ImGui::BeginMenuBar()) {
        
        
        if(ImGui::BeginMenu("File")) {
            if(ImGui::MenuItem("Open", "Ctrl-O", nullptr))
                OpenFile();
            if(m_SelectedBuffer > -1) {
                if(ImGui::MenuItem("Save", "Ctrl-S", nullptr))
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


void TextEditorPanel::DrawCloseConfirmationWindow() {
    if (ImGui::BeginPopupModal("Save?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        int bufferToClose = m_BuffersToCloseWithConfiramtion[m_BuffersToCloseWithConfiramtion.size() - 1];
        m_Buffers[bufferToClose].ShouldShow = true;
        m_ConfirmationWindowDisplayed = true;
        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

        ImGui::Text("Do you want to save %s file?", m_Buffers[bufferToClose].GetTitle().c_str());
        ImGui::Separator();

        if (ImGui::Button("Save", ImVec2(120, 0))) {
            m_Buffers[bufferToClose].SaveFile();
            CloseBuffer(bufferToClose);
            ImGui::CloseCurrentPopup();
            m_ConfirmationWindowDisplayed = false;
            m_BuffersToCloseWithConfiramtion.pop_back();
        }
        ImGui::SameLine();
        if (ImGui::Button("Don't save", ImVec2(120, 0))) {
            CloseBuffer(bufferToClose);
            ImGui::CloseCurrentPopup();
            m_ConfirmationWindowDisplayed = false;
            m_BuffersToCloseWithConfiramtion.pop_back();
        }

        ImGui::SetItemDefaultFocus();
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(120, 0))) {
            ImGui::CloseCurrentPopup();
            m_ConfirmationWindowDisplayed = false;
            m_BuffersToCloseWithConfiramtion.clear();
        }

        ImGui::EndPopup();
    }
    
    if(m_BuffersToCloseWithConfiramtion.size() > 0 && m_ConfirmationWindowDisplayed == false)
        ImGui::OpenPopup("Save?");
}


TextBuffer::TextBuffer(const Core::Ref<Core::Utils::File>& file) {
    auto lang = TextEditor::LanguageDefinition::GLSL();
    Buffer.SetLanguageDefinition(lang);
    Buffer.SetShowWhitespaces(false);
    Buffer.SetPalette(TextEditor::GetLightPalette());
    if(file != nullptr) {
        if(!OpenFile(file))
            Buffer.SetText("");
    } else {
        Buffer.SetText("");
    }
    
    auto id = (uint64_t)this;
    bufferID = std::to_string(id);
}

void TextBuffer::Draw() {
    Buffer.Render(bufferID.c_str());
    if(Buffer.IsTextChanged())
        TextChanged = true;
}

bool TextBuffer::OpenFile(const Core::Ref<Core::Utils::File>& file) {
    if(file != nullptr) {
        auto fileSrc = file->Read();
        if(fileSrc.has_value()) {
            Buffer.SetText(fileSrc.value());
            auto lang = TextEditor::LanguageDefinition::GLSL();
            Buffer.SetLanguageDefinition(lang);
            ShouldShow = true;
            File = file;
            return true;
        }
    } else {
        auto path = Core::Utils::FileDialogs::OpenFile("");
        if(!path.empty()) {
            auto file = Core::Utils::FileWatcher::Get().LoadFile(path);
            return OpenFile(file);
        }
    }
    
    return false;
}

void TextBuffer::SaveFile() {
    if(!TextChanged) return;
    
    if(File == nullptr) {
        std::string savePath = Core::Utils::FileDialogs::SaveFile("");
        if(!savePath.empty())
            File = Core::Utils::FileWatcher::Get().LoadFile(savePath);
        else
            return;
    }
    std::string stringToSave = Buffer.GetText();
    stringToSave.pop_back();
    
    File->Write(stringToSave);
    TextChanged = false;
}

std::string TextBuffer::GetTitle() {
    return File == nullptr ? "New File" : File->GetPath().filename();
}


}
