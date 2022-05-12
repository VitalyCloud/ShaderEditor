//
//  MessagePanel.cpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 12.05.2022.
//

#include "MessagePanel.hpp"


namespace Editor {

MessagePanel* MessagePanel::s_MessagePanel = nullptr;

MessagePanel::MessagePanel() {
    EN_ASSERT(s_MessagePanel == nullptr, "Message panel is already created");
    s_MessagePanel = this;
}

MessagePanel::~MessagePanel() {
    
}

void MessagePanel::Clear() {
    m_Buffer.clear();
    m_LineOffsets.clear();
    m_LineOffsets.push_back(0);
}

void MessagePanel::AddLog(const char* fmt, ...) {
    int old_size = m_Buffer.size();
    va_list args;
    va_start(args, fmt);
    m_Buffer.appendfv(fmt, args);
    va_end(args);
    for (int new_size = m_Buffer.size(); old_size < new_size; old_size++)
        if (m_Buffer[old_size] == '\n')
            m_LineOffsets.push_back(old_size + 1);
}

void MessagePanel::SetMessage(ShaderPass* shaderPass, const std::string& message) {
    m_Messages[shaderPass] = message;
}

void MessagePanel::DeleteMessage(ShaderPass* shaderPass) {
    auto it = m_Messages.find(shaderPass);
    if(it != m_Messages.end())
        m_Messages.erase(it);
}

void MessagePanel::Draw(const char* title, bool* p_open) {
    if (!ImGui::Begin(title, p_open)) {
        ImGui::End();
        return;
    }
    
    Clear();
    for(auto const& x: m_Messages) {
        if(x.first == nullptr) {
            m_Messages.erase(x.first);
            continue;
        }
        AddLog("%s: %s", x.first->GetTitle().c_str(), x.second.c_str());
    }
        
    
    
    
    ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
    
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
    const char* buf = m_Buffer.begin();
    const char* buf_end = m_Buffer.end();
    
    ImGuiListClipper clipper;
    clipper.Begin(m_LineOffsets.Size);
    while (clipper.Step())
    {
        for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++)
        {
            const char* line_start = buf + m_LineOffsets[line_no];
            const char* line_end = (line_no + 1 < m_LineOffsets.Size) ? (buf + m_LineOffsets[line_no + 1] - 1) : buf_end;
            ImGui::TextUnformatted(line_start, line_end);
        }
    }
    clipper.End();
    ImGui::PopStyleVar();
    
    if (m_AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
        ImGui::SetScrollHereY(1.0f);
    
    ImGui::EndChild();
    ImGui::End();
}

}
