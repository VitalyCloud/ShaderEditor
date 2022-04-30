//
//  Input.cpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 21.04.2022.
//
#include "pch.h"
#include "Input.hpp"
#include "Core/Application.hpp"

#include "GLFW/glfw3.h"

namespace Core {

bool Input::IsKeyPressed(KeyCode key)  {
    GLFWwindow* window = Application::Get().GetWindow();
    auto state = glfwGetKey(window, key);
    return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool Input::IsMouseButtonPressed(MouseCode button) {
    GLFWwindow* window = Application::Get().GetWindow();
    auto state = glfwGetMouseButton(window, button);
    return state == GLFW_PRESS;
}

std::pair<float, float> Input::GetMousePosition() {
    GLFWwindow* window = Application::Get().GetWindow();
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    return { (float)xpos, (float)ypos };
}

float Input::GetMouseX() {
    auto [x, y] = GetMousePosition();
    return x;
}

float Input::GetMouseY() {
    auto [x, y] = GetMousePosition();
    return y;
}

float Input::GetTime() {
    return (float)glfwGetTime();
}

}
