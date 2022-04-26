//
//  TextEditorPanel.hpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 22.04.2022.
//

#ifndef TextEditorPanel_hpp
#define TextEditorPanel_hpp

#include "Core/Core.hpp"
#include "TextBufferView.hpp"
#include <vector>
#include <string>

namespace Editor {

class TextEditorPanel {
public:
    TextEditorPanel();
    ~TextEditorPanel();
    
    void Draw(const char* title, bool* p_open = nullptr);
    void OnEvent(Core::Event& event);
    
    void AddBuffer();
    void AddBufer(const Core::Ref<Core::Utils::File>& file);
    
    void CloseBuffer(Core::Ref<TextBufferView> buffer);
    
    bool IsFileOpened(const Core::Ref<Core::Utils::File>& file);
    
private:
    void DrawConfirmationWindow();
    
private:
    std::vector<Core::Ref<TextBufferView>> m_Buffers;
    std::vector<Core::Ref<TextBufferView>> m_BuffersToClose;
    Core::Ref<TextBufferView> m_SelectedBuffer = nullptr;
    
    bool m_ConfirmationWindowDisplayed = false;
    ImFont* m_Font;
};


}

#endif /* TextEditorPanel_hpp */
