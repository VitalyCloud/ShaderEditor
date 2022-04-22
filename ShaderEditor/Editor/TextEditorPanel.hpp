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
    void OnEvent(Engine::Event& event);
    
    void AddBuffer(const std::string filepath);
    void AddBuffer();
    
    void CloseBuffer(Engine::Ref<TextBufferView> buffer);
    
private:
    void DrawConfirmationWindow();
    
private:
    std::vector<Engine::Ref<TextBufferView>> m_Buffers;
    std::vector<Engine::Ref<TextBufferView>> m_BuffersToClose;
    Engine::Ref<TextBufferView> m_SelectedBuffer = nullptr;
    
    bool m_ConfirmationWindowDisplayed = false;
};


}

#endif /* TextEditorPanel_hpp */
