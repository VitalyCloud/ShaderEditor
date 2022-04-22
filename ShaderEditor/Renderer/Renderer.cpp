//
//  Renderer.cpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 21.04.2022.
//
#include "Core/pch.h"
#include "Renderer.hpp"

namespace Renderer {

    struct SceneData {
        glm::mat4 ViewProjectionMatrix;
    };

    static Engine::Scope<SceneData> s_SceneData = Engine::CreateScope<SceneData>();

    void OnWindowResize(uint32_t width, uint32_t height) {
        OpenGL::RenderCommand::SetViewport(0, 0, width, height);
    }

    void BeginScene(const OrthographicCamera& camera) {
        s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
    }

    void EndScene() {
        
    }

    void Submit(const Engine::Ref<OpenGL::Shader> shader, const Engine::Ref<OpenGL::VertexArray>& vertexArray) {
        
        shader->Bind();
        shader->SetMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
        
        vertexArray->Bind();
        OpenGL::RenderCommand::DrawIndexed(vertexArray);
    }

}



