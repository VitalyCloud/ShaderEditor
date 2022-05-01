//
//  InspectorPanel.hpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 01.05.2022.
//

#ifndef InspectorPanel_hpp
#define InspectorPanel_hpp

namespace Editor {

class Inspector {
public:
    virtual void Draw() = 0;
};

class InspectorPanel {
public:
    InspectorPanel();
    ~InspectorPanel();
    
    void SetContext(Inspector* context = nullptr) { m_Context = context;}
    const Inspector* GetContext() const { return m_Context; }
    bool IsActive(Inspector* context) { return m_Context == context; }
  
    void Draw(const char* title, bool* p_open = nullptr) const;
    
    static InspectorPanel& Get() { return *s_InspectorPanel; }
private:
    static InspectorPanel* s_InspectorPanel;
    Inspector* m_Context = nullptr;
};

}



#endif /* InspectorPanel_hpp */
