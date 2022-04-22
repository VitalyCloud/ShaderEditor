//
//  RenderCommand.cpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 21.04.2022.
//
#include "Core/pch.h"
#include "RenderCommand.hpp"
#include "glad/glad.h"

namespace OpenGL {

    namespace RenderCommand {
        void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
            glViewport(x, y, width, height);
        }

        void SetClearColor(const glm::vec4& color) {
            glClearColor(color.r, color.g, color.b, color.a);
        }

        void Clear() {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }
    
        void DrawIndexed(const Engine::Ref<VertexArray>& vertexArray, uint32_t indexCount) {
            
            uint32_t count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount();
            glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
        }
    }

}
