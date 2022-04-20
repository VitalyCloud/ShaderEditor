//
//  Core.h
//  ShaderEditor
//
//  Created by Vitaly Cloud on 20.04.2022.
//

#ifndef Core_hpp
#define Core_hpp

#include "Application.hpp"
#include "EntryPoint.hpp"
#include "Layer.hpp"
#include "Events/Event.hpp"
#include "Events/ApplicationEvent.hpp"
#include "Events/MouseEvent.hpp"
#include "Events/KeyEvent.hpp"
#include "Input/Input.hpp"
#include "Input/KeyCodes.hpp"
#include "Input/MouseCodes.hpp"
#include "glm/glm.hpp"

#include "imgui.h"

#include <memory>
#define BIND_EVENT_FN(x) std::bind(x, this, std::placeholders::_1)

namespace Engine {
    
    template<typename T>
    using Scope = std::unique_ptr<T>;
    template<typename T, typename ... Args>
    constexpr Scope<T> CreateScope(Args&& ... args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }
    
    template<typename T>
    using Ref = std::shared_ptr<T>;
    template<typename T, typename ... Args>
    constexpr Ref<T> CreateRef(Args&& ... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }
}

#endif /* Core_hpp */
