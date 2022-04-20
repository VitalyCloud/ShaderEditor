#pragma once

#include "KeyCodes.hpp"
#include "MouseCodes.hpp"

#include "KeyCodes.hpp"
#include "MouseCodes.hpp"

namespace Engine {

    class Input
    {
    public:
        static bool IsKeyPressed(KeyCode key);
        static bool IsMouseButtonPressed(MouseCode button);
        static std::pair<float, float> GetMousePosition();
        static float GetMouseX();
        static float GetMouseY();

        static float GetTime();

    };

}
