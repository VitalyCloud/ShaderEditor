#pragma once

#include "imgui.h"
#include <string>

#include "UniformBuffer.hpp"

#include "ImGuiInputSettings.h"

namespace ImGui {

bool InputText(const char* label, std::string* str, ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = NULL, void* user_data = NULL);


bool DrawInputFloat(float* value, const InputSettings* settings = nullptr, int count = 1);
bool DrawInputInt(int* value, const InputSettings* settings = nullptr, int count = 1);
bool DrawInputMat(float* value, const InputSettings* settings = nullptr, int count = 3);

}
