//
//  TextBufferView.cpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 22.04.2022.
//

#include "TextBufferView.hpp"
#include "Core/Utils.hpp"
#include <filesystem>

namespace Editor {

TextBufferView::TextBufferView() {
    auto lang = TextEditor::LanguageDefinition::GLSL();
    m_Buffer.SetLanguageDefinition(lang);
    m_Buffer.SetShowWhitespaces(false);
    m_Buffer.SetPalette(TextEditor::GetLightPalette());
    m_Buffer.SetText("");
}

TextBufferView::~TextBufferView() {
    
}

void TextBufferView::Draw() {
    auto cpos = m_Buffer.GetCursorPosition();
    ImGui::SetWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);
    
    ImGui::Text("%6d/%-6d %6d lines  | %s | %s | %s | %s", cpos.mLine + 1, cpos.mColumn + 1, m_Buffer.GetTotalLines(),
        m_Buffer.IsOverwrite() ? "Ovr" : "Ins",
        m_Buffer.CanUndo() ? "*" : " ",
        m_Buffer.GetLanguageDefinition().mName.c_str(), m_CurrentFilePath.c_str());
    
    DrawMenuBar();

    auto id = (uint64_t)this;
    std::string bufferTitle = std::to_string(id);

    
//    ImGui::PushFont(font);
    m_Buffer.Render(bufferTitle.c_str());
//    ImGui::PopFont();

    if (m_Buffer.IsTextChanged()) {
        m_TextChanged = true;
    }
}

void TextBufferView::OnEvent(Engine::Event& event) {
    Engine::EventDispatcher dispatcher(event);
    dispatcher.Dispatch<Engine::KeyPressedEvent>(BIND_EVENT_FN(&TextBufferView::OnKeyPressed));
}

const std::string TextBufferView::GetTitle() {
    if(m_CurrentFilePath.empty()) {
        return "New File";
    } else {
        std::filesystem::path path(m_CurrentFilePath);
        return path.filename();
    }
}

bool TextBufferView::OnKeyPressed(Engine::KeyPressedEvent& event) {
    if (event.GetRepeatCount() > 0)
        return false;
    bool control = Engine::Input::IsKeyPressed(Engine::Key::LeftControl) ||
        Engine::Input::IsKeyPressed(Engine::Key::RightControl);
    switch (event.GetKeyCode()) {
    case Engine::Key::S:
        if (control)
            SaveFile();
        break;
    case Engine::Key::O:
        if (control) {
            OpenFile();
        }
    default:
        break;
    }
    return true;
}
                                    

void TextBufferView::DrawMenuBar() {
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Open", "Ctrl-O")) {
                OpenFile();
            }

            if (ImGui::MenuItem("Save", "Ctrl-S"))
                SaveFile();
            
            if (ImGui::MenuItem("Close"))
                m_ShouldClose = true;
                
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {
            bool ro = m_Buffer.IsReadOnly();
            if (ImGui::MenuItem("Read-only mode", nullptr, &ro))
                m_Buffer.SetReadOnly(ro);
            ImGui::Separator();

            if (ImGui::MenuItem("Undo", "ALT-Backspace", nullptr, !ro && m_Buffer.CanUndo()))
                m_Buffer.Undo();
            if (ImGui::MenuItem("Redo", "Ctrl-Y", nullptr, !ro && m_Buffer.CanRedo()))
                m_Buffer.Redo();

            ImGui::Separator();

            if (ImGui::MenuItem("Copy", "Ctrl-C", nullptr, m_Buffer.HasSelection()))
                m_Buffer.Copy();
            if (ImGui::MenuItem("Cut", "Ctrl-X", nullptr, !ro && m_Buffer.HasSelection()))
                m_Buffer.Cut();
            if (ImGui::MenuItem("Delete", "Del", nullptr, !ro && m_Buffer.HasSelection()))
                m_Buffer.Delete();
            if (ImGui::MenuItem("Paste", "Ctrl-V", nullptr, !ro && ImGui::GetClipboardText() != nullptr))
                m_Buffer.Paste();

            ImGui::Separator();

            if (ImGui::MenuItem("Select all", nullptr, nullptr))
                m_Buffer.SetSelection(TextEditor::Coordinates(), TextEditor::Coordinates(m_Buffer.GetTotalLines(), 0));

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("View"))
        {
            if (ImGui::MenuItem("Dark palette"))
                m_Buffer.SetPalette(TextEditor::GetDarkPalette());
            if (ImGui::MenuItem("Light palette"))
                m_Buffer.SetPalette(TextEditor::GetLightPalette());
            if (ImGui::MenuItem("Retro blue palette"))
                m_Buffer.SetPalette(TextEditor::GetRetroBluePalette());
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
}

bool TextBufferView::OpenFile(const std::string &filepath) {
    if(!filepath.empty()) {
        auto file = Engine::Utils::FileSystem::ReadFile(filepath);
        if(file.has_value()) {
            m_CurrentFilePath = filepath;
            m_Buffer.SetText(file.value());
            auto lang = TextEditor::LanguageDefinition::GLSL();
            m_Buffer.SetLanguageDefinition(lang);
            m_ShouldShow = true;
            return true;
        }
    } else {
        auto path = Engine::Utils::FileDialogs::OpenFile("");
        if(!path.empty()) {
            return OpenFile(path);
        }
    }
    
    return false;
}

void TextBufferView::SaveFile() {
    if(m_CurrentFilePath.empty())
        m_CurrentFilePath = Engine::Utils::FileDialogs::SaveFile("");
    if(!m_CurrentFilePath.empty()) {
        Engine::Utils::FileSystem::WriteFile(m_Buffer.GetText(), m_CurrentFilePath);
        m_TextChanged = false;
    }
        
}


}


