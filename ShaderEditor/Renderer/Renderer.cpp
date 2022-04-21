//
//  Renderer.cpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 21.04.2022.
//

#include "Renderer.hpp"

namespace Renderer {

void OnWindowResize(uint32_t width, uint32_t height) {
    OpenGL::RenderCommand::SetViewport(0, 0, width, height);
}

void BeginScene() {
    
}

void EndScene() {
    
}

void Submit(const Engine::Ref<OpenGL::Shader> shader, const Engine::Ref<OpenGL::VertexArray>& vertexArray) {
    
    shader->Bind();
    vertexArray->Bind();
    OpenGL::RenderCommand::DrawIndexed(vertexArray);
}

}
