//
//  TextBufferView.hpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 22.04.2022.
//

#ifndef TextBufferView_hpp
#define TextBufferView_hpp

#include <string>

#include "Extensions/TextEdit/TextEditor.h"
#include "Core/Core.hpp"

namespace Editor {
    
class TextBufferView {
public:
    TextBufferView();
    ~TextBufferView();
    
    void Draw();
    void OnEvent(Engine::Event& event);
    
    bool OpenFile(const std::string& filepath = "");
    void SaveFile();
    
    const std::string GetTitle();
    bool IsTextChanged() { return m_TextChanged; }
    bool& ShouldShow() { return m_ShouldShow; }
    bool& ShouldClose() { return m_ShouldClose; }
    
private:
    void DrawMenuBar();
    bool OnKeyPressed(Engine::KeyPressedEvent& event);
    
private:
    TextEditor m_Buffer;
    std::string m_CurrentFilePath;
    bool m_TextChanged = false;
    bool m_ShouldShow = false;
    bool m_ShouldClose = false;
};

}


#endif /* TextBufferView_hpp */
