//
//  RenderCommand.cpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 21.04.2022.
//
#include "pch.h"
#include "RenderCommand.hpp"
#include "glad/glad.h"

namespace OpenGL {

static GLenum OpenGLPrimitive(Primitive type) {
    switch(type) {
        case Primitive::Points:
            return GL_POINTS;
        case Primitive::Lines:
            return GL_LINES;
        case Primitive::LineStrip:
            return GL_LINE_STRIP;
        case Primitive::LineLoop:
            return GL_LINE_LOOP;
        case Primitive::Triangles:
            return GL_TRIANGLES;
        case Primitive::TriangleStrip:
            return GL_TRIANGLE_STRIP;
        case Primitive::TrinagleFan:
            return GL_TRIANGLE_FAN;
        default:
            EN_ASSERT(false, "Unknown Primitive");
    }
    return 0;
}

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
    
        void DrawIndexed(const Core::Ref<VertexArray>& vertexArray, Primitive topology, uint32_t indexCount) {
            uint32_t count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount();
            glDrawElements(OpenGLPrimitive(topology), count, GL_UNSIGNED_INT, nullptr);
        }
    
        void Draw(uint32_t count, Primitive topology) {
            glDrawArrays(OpenGLPrimitive(topology), 0, count);
        }
    }

}
