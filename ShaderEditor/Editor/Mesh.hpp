//
//  Mesh.hpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 01.05.2022.
//

#ifndef Mesh_hpp
#define Mesh_hpp

#include "InspectorPanel.hpp"
#include "VertexArrayContainer.hpp"
#include "VertexArrayView.hpp"
#include "ShaderPass.hpp"

#include "Components.hpp"

#include "ImGuiHelper.hpp"


namespace Editor {

class Mesh;

class MeshInspector: public Inspector {
public:
    MeshInspector();
    void Draw() override;
    void SetContext(Mesh* context) { m_Context = context; }
    const Mesh* GetContext() { return m_Context; }
private:
    Mesh* m_Context = nullptr;
    VertexArrayView m_VertexArrayView;
    
    ImGuiInputSettings m_PositionSettings;
    ImGuiInputSettings m_RrotationSettings;
    ImGuiInputSettings m_ScaleSettings;
    ImGuiInputSettings* m_PopupContext = nullptr;
};

class DefaultMesh {
public:
    enum MeshType {
        Triangle, Quad
    };
    
    static Core::Ref<VertexArrayContainer> CreateTriangle();
    static Core::Ref<VertexArrayContainer> CreateQuad();
};

class Mesh: public ShaderPassObject {
public:
    Mesh(const std::string& title = "Mesh");
    Mesh(DefaultMesh::MeshType type, const std::string& title = "");
    ~Mesh();
    
    void OnUpdate(const Core::Ref<OpenGL::Shader>& shader) override;
    std::string& GetTitle() override { return m_Title; }
    
private:
    friend MeshInspector;
    std::string m_Title;
    Core::Ref<VertexArrayContainer> m_VertexArrayContainer;
    
    bool m_UseIndexBuffer = true;
    
    TransformComponent m_Transform;
};



}

#endif /* Mesh_hpp */
