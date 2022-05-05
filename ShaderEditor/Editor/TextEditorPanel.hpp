//
//  TextEditorPanel.hpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 05.05.2022.
//

#ifndef TextEditorPanel_hpp
#define TextEditorPanel_hpp

#include "Core/Core.hpp"

#include "Extensions/TextEdit/TextEditor.h"

#include <vector>

namespace Editor {

struct TextBuffer {
    TextEditor Buffer;
    Core::Ref<Core::Utils::File> File = nullptr;
    bool TextChanged = false;
    bool ShouldShow = false;
    bool ShouldClose = false;
    
    std::string bufferID;
    
    TextBuffer(const Core::Ref<Core::Utils::File>& file = nullptr) {
        auto lang = TextEditor::LanguageDefinition::GLSL();
        Buffer.SetLanguageDefinition(lang);
        Buffer.SetShowWhitespaces(false);
        Buffer.SetPalette(TextEditor::GetDarkPalette());
        if(file != nullptr) {
            if(!OpenFile(file))
                Buffer.SetText("");
        } else {
            Buffer.SetText("");
        }
        
        auto id = (uint64_t)this;
        bufferID = std::to_string(id);
    }
    
    void Draw() {
        Buffer.Render(bufferID.c_str());
        if(Buffer.IsTextChanged())
            TextChanged = true;
    }
    
    bool OpenFile(const Core::Ref<Core::Utils::File>& file = nullptr) {
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
    
    void SaveFile() {
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
    
    std::string GetTitle() {
        return File == nullptr ? "New File" : File->GetPath().filename();
    }
};

class TextEditorPanel {
    
public:
    TextEditorPanel();
    ~TextEditorPanel();
  
    void Draw(const char* title, bool* p_open = nullptr);
    void OnEvent(Core::Event& event);
    
    void OpenFile(const Core::Ref<Core::Utils::File>& file = nullptr);
    
private:
    void DrawMenuBar();
    int GetIndexForFile(const Core::Ref<Core::Utils::File>& file);
    bool OnKeyPressed(Core::KeyPressedEvent& event);
    
private:
    ImFont* m_Font = nullptr;
    std::vector<TextBuffer> m_Buffers;
    int m_SelectedBuffer = -1;
    
    
};

}

#endif /* TextEditorPanel_hpp */
