//
//  ShaderPass.cpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 26.04.2022.
//

#include "Core/pch.h"
#include "ShaderPass.hpp"

namespace Editor {

ShaderPass::ShaderPass(const std::string& title) : m_Title(title) {
    std::string vertexSrc = R"(
        #version 330 core

        layout(location = 0) in vec2 a_Position;
        layout(location = 1) in vec3 a_Color;

        uniform mat4 u_ViewProjection;
    
        out vec3 v_Color;
        void main()
        {
            v_Color = a_Color;
            gl_Position = u_ViewProjection * vec4(a_Position, 1.0f, 1.0f);
        }
    )";

    std::string fragmentSrc = R"(
        #version 330 core
        layout(location = 0) out vec4 color;
        in vec3 v_Color;
        void main()
        {
            color = vec4(v_Color, 1.0f);
        }
    )";

    m_Shader = Core::CreateRef<OpenGL::Shader>(vertexSrc, fragmentSrc);
}

ShaderPass::~ShaderPass() {
    
}

void ShaderPass::OnUpdate() {
    
    m_Shader->Bind();
    m_VertexArray.GetOpenGLVA()->Bind();
//    OpenGL::RenderCommand::Draw(m_VertexArray.GetVertexCount());
    OpenGL::RenderCommand::DrawIndexed(m_VertexArray.GetOpenGLVA());
}

}
