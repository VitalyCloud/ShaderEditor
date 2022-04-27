//
//  InspectorPanel.hpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 27.04.2022.
//

#ifndef InspectorPanel_hpp
#define InspectorPanel_hpp

#include "Core/Core.hpp"

namespace Editor {

class InspectorItem {
public:
    virtual void Draw() = 0;
};

class InspectorPanel {
public:
    InspectorPanel();
    ~InspectorPanel();
    
    void SetContext(InspectorItem* item) { m_Context = item; }
    InspectorItem* GetContext() { return m_Context; }
    bool IsActive(InspectorItem* item) { return m_Context == item; }
    
    void Draw(const char* title, bool* p_open = nullptr);
    
    static InspectorPanel& Get() { return *s_InspectorPanel; }
private:
    static InspectorPanel* s_InspectorPanel;
    InspectorItem* m_Context = nullptr;
};

}

#endif /* InspectorPanel_hpp */
