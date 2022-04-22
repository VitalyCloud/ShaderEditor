//
//  Renderer.hpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 21.04.2022.
//

#ifndef Renderer_hpp
#define Renderer_hpp

#include <stdint.h>

#include "Core/Core.hpp"
#include "OpenGL/OpenGL.hpp"
#include "OrthographicCamera.hpp"

namespace Renderer {

    void OnWindowResize(uint32_t width, uint32_t height);

    void BeginScene(const OrthographicCamera& camera);

    void EndScene();

    void Submit(const Engine::Ref<OpenGL::Shader> shader, const Engine::Ref<OpenGL::VertexArray>& vertexArray);

}









#endif /* Renderer_hpp */
