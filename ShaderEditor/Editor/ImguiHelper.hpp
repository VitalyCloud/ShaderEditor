//
//  ImguiHelper.hpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 01.05.2022.
//

#ifndef ImguiHelper_hpp
#define ImguiHelper_hpp

#include "OpenGL/Buffer.hpp"

#include "imgui.h"
#include <string>

enum class ImGuiWidgetType {
    Input = 0, Drag, Slider, Color
};

struct ImGuiInputSettings {
    bool UseRange = false;
    float Min = -10.0f;
    float Max = +10.f;
    float Speed = 0.1;
    ImGuiWidgetType Type = ImGuiWidgetType::Input;
};

namespace ImGui {
bool InputText(const char* label, std::string* str, ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = NULL, void* user_data = NULL);

    
void DrawInputSettingsView(OpenGL::ShaderDataType type, ImGuiInputSettings* settings = nullptr);
bool DrawInputFloat(float* value, const ImGuiInputSettings* settings = nullptr, int count = 1);
bool DrawInputInt(int* value, const ImGuiInputSettings* settings = nullptr, int count = 1);
bool DrawInputMat(float* value, const ImGuiInputSettings* settings = nullptr, int count = 3);
}



#endif /* ImguiHelper_hpp */
