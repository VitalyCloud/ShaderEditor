//
//  Core.h
//  ShaderEditor
//
//  Created by Vitaly Cloud on 20.04.2022.
//

#ifndef Core_hpp
#define Core_hpp

#include "PlatformDetection.h"
#include "Log.hpp"
#include "Assert.h"
#include "Application.hpp"
#include "Layer.hpp"
#include "Events/Event.hpp"
#include "Events/ApplicationEvent.hpp"
#include "Events/MouseEvent.hpp"
#include "Events/KeyEvent.hpp"
#include "Input/Input.hpp"
#include "Input/KeyCodes.hpp"
#include "Input/MouseCodes.hpp"
#include "Memory.hpp"


#include "glm/glm.hpp"
#include "imgui.h"

#define BIND_EVENT_FN(x) std::bind(x, this, std::placeholders::_1)



#endif /* Core_hpp */
