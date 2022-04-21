//
//  RenderCommand.hpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 21.04.2022.
//

#ifndef RenderCommand_hpp
#define RenderCommand_hpp

#include "stdint.h"
#include "glm/glm.hpp"

namespace OpenGL {

    namespace RenderCommand {
    
        void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);

        void SetClearColor(const glm::vec4& color);

        void Clear();

        void EnableDepthTesting();
    }

}

#endif /* RenderCommand_hpp */
