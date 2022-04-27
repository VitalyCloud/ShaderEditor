//
//  Mesh.hpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 27.04.2022.
//

#ifndef Mesh_hpp
#define Mesh_hpp

#include "Core/Core.hpp"
#include "VertexBufferConteiner.hpp"
#include "OpenGL/OpenGL.hpp"

#include "InspectorPanel.hpp"

namespace Editor {

class MeshInspector;

class Mesh {
public:
    using VertexBufferContainer = std::vector<uint32_t>;
    Mesh(const std::string& title = "New Mesh") : m_Title(title) {}
    ~Mesh() {}
    
    const Core::Ref<OpenGL::VertexArray>& GetVertexArray() { return m_VertexArray; }
    uint32_t GetVertexCount() { return m_VertexBuffers[0]->VertexCount(); }
    const std::string& GetTitle() { return m_Title; }
    
private:
    friend class MeshInspector;
    std::vector<Core::Ref<VertexBufferConteiner>> m_VertexBuffers;
    Core::Ref<VertexBufferContainer> m_Indicies;
    Core::Ref<OpenGL::VertexArray> m_VertexArray;
    
    std::string m_Title;
};

class MeshInspector: public InspectorItem {
public:
    void Draw() override;
    void SetContext(const Core::Ref<Mesh>& context)  { m_Context = context;  }
    const Core::Ref<Mesh>& GetContext() { return m_Context; }
private:
    Core::Ref<Mesh> m_Context;
};


}


#endif /* Mesh_hpp */
