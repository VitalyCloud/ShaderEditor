//
//  ImGuiInputSettings.hpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 24.04.2022.
//

#ifndef ImGuiInputSettings_hpp
#define ImGuiInputSettings_hpp

#include "OpenGL/Buffer.hpp"

namespace Editor {

enum class ImGuiWidgetType {
    Input = 0, Drag, Slider, Color
};

struct InputSettings {
    bool UseRange = false;
    float Min = -10.0f;
    float Max = +10.f;
    float Speed = 0.1;
    ImGuiWidgetType Type = ImGuiWidgetType::Input;
};

void DrawInputSettingsView(OpenGL::ShaderDataType type, InputSettings* settings = nullptr);

bool DrawInputFloat(float* value, const InputSettings* settings = nullptr, int count = 1);
bool DrawInputInt(int* value, const InputSettings* settings = nullptr, int count = 1);
bool DrawInputMat(float* value, const InputSettings* settings = nullptr, int count = 3);

}



#endif /* ImGuiInputSettings_hpp */
