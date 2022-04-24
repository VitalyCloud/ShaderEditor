//
//  ImGuiInputSettings.h
//  ShaderEditor
//
//  Created by Vitaly Cloud on 24.04.2022.
//

#ifndef ImGuiInputSettings_h
#define ImGuiInputSettings_h

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

#endif /* ImGuiInputSettings_h */
