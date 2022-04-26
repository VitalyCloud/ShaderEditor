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
    
    void Draw(ImFont* font = ImGui::GetIO().FontDefault);
    void OnEvent(Core::Event& event);
    
    bool OpenFile(const Core::Ref<Core::Utils::File>& file = nullptr);
    void SaveFile();
    
    const std::string GetTitle();
    const Core::Ref<Core::Utils::File>& GetFile() { return m_File; }
    bool IsTextChanged() { return m_TextChanged; }
    bool& ShouldShow() { return m_ShouldShow; }
    bool& ShouldClose() { return m_ShouldClose; }
    
private:
    void DrawMenuBar();
    bool OnKeyPressed(Core::KeyPressedEvent& event);
    
private:
    TextEditor m_Buffer;
    Core::Ref<Core::Utils::File> m_File = nullptr;
 
    bool m_TextChanged = false;
    bool m_ShouldShow = false;
    bool m_ShouldClose = false;
};

}


#endif /* TextBufferView_hpp */
