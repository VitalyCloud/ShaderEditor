#pragma once

#include "imgui.h"
#include <string>

#include "UniformBufferConteiner.hpp"
#include "ImGuiInputSettings.hpp"

namespace ImGui {

bool InputText(const char* label, std::string* str, ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = NULL, void* user_data = NULL);

}
