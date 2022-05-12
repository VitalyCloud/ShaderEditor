//
//  MessagePanel.hpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 12.05.2022.
//

#ifndef MessagePanel_hpp
#define MessagePanel_hpp

#include <stdio.h>
#include <map>

#include "ShaderPass.hpp"

#include "imgui.h"

namespace Editor {

class MessagePanel {
public:
    MessagePanel();
    ~MessagePanel();
    
    void Draw(const char* title, bool* p_open);
    void SetAutoScroll(bool value) { m_AutoScroll = value; }
    void SetMessage(ShaderPass* shaderPass, const std::string& message);
    void DeleteMessage(ShaderPass* shaderPass);
    
    static MessagePanel& Get() { return *s_MessagePanel; }
    
private:
    void AddLog(const char* fmt, ...);
    void Clear();
    
private:
    static MessagePanel* s_MessagePanel;
    
    std::map<ShaderPass*, std::string> m_Messages;
    ImVector<int> m_LineOffsets;
    ImGuiTextBuffer m_Buffer;
    bool m_AutoScroll;
    
};

}



#endif /* MessagePanel_hpp */
