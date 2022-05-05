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
    
    TextBuffer(const Core::Ref<Core::Utils::File>& file = nullptr);
    void Draw();
    bool OpenFile(const Core::Ref<Core::Utils::File>& file = nullptr);
    void SaveFile();
    std::string GetTitle();
};

class TextEditorPanel {
    
public:
    TextEditorPanel();
    ~TextEditorPanel();
  
    void Draw(const char* title, bool* p_open = nullptr);
    void OnEvent(Core::Event& event);
    
    void OpenFile(const Core::Ref<Core::Utils::File>& file = nullptr);
    void CloseBuffer(int index);
    void AddBuffer();
    
    static TextEditorPanel& Get() { return *s_TextEditorPanel; }
private:
    void DrawMenuBar();
    void DrawCloseConfirmationWindow();
    int GetIndexForFile(const Core::Ref<Core::Utils::File>& file);
    bool OnKeyPressed(Core::KeyPressedEvent& event);
    
private:
    static TextEditorPanel* s_TextEditorPanel;
    ImFont* m_Font = nullptr;
    std::vector<TextBuffer> m_Buffers;
    int m_SelectedBuffer = -1;
    std::vector<int> m_BuffersToCloseWithConfiramtion;
    std::vector<int> m_BuffersToClose;
    bool m_ConfirmationWindowDisplayed = false;
    
    
};

}

#endif /* TextEditorPanel_hpp */
