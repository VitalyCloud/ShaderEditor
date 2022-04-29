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
#include "VertexView.hpp"
#include "IndexView.hpp"

namespace Editor {

class MeshInspector;

enum class DefaultMesh {
    Triangle, Squad
};

class Mesh {
public:
    using IndexBufferContainer = std::vector<uint32_t>;
    Mesh(const std::string& title = "New Mesh");
    Mesh(DefaultMesh defaultMesh, const std::string& title = "");
    ~Mesh();
    
    void Draw(const Core::Ref<OpenGL::Shader>& shader);
    
    const Core::Ref<OpenGL::VertexArray>& GetVertexArray() { return m_VertexArray; }
    uint32_t GetVertexCount() { return m_VertexBuffers[0]->VertexCount(); }
    const std::string& GetTitle() { return m_Title; }
    
private:
    void InvalidateVertexArray();
    void AddVertexBuffer();
    void RemoveVertexBuffer(int index);
    void PopulateDefaultMesh(DefaultMesh defaultMesh);

private:
    friend class MeshInspector;
    std::vector<Core::Ref<VertexBufferConteiner>> m_VertexBuffers;
    Core::Ref<IndexBufferContainer> m_Indicies;
    Core::Ref<OpenGL::VertexArray> m_VertexArray;
    bool m_DrawIndexed = false;
    
    std::vector<Core::Ref<OpenGL::Texture>> m_Textures;
    
    std::string m_Title;
};

class MeshInspector: public InspectorItem {
public:
    void Draw() override;
    void SetContext(const Core::Ref<Mesh>& context)  { m_Context = context;  }
    const Core::Ref<Mesh>& GetContext() { return m_Context; }
private:
    Core::Ref<Mesh> m_Context;
    
    VertexView m_VertexView;
    IndexView m_IndexView;
};


}


#endif /* Mesh_hpp */
