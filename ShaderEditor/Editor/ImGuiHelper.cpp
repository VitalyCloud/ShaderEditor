#include "Core/pch.h"
#include "ImGuiHelper.h"

namespace ImGui {
	struct InputTextCallback_UserData
	{
		std::string* Str;
		ImGuiInputTextCallback  ChainCallback;
		void* ChainCallbackUserData;
	};

	static int InputTextCallback(ImGuiInputTextCallbackData* data)
	{
		InputTextCallback_UserData* user_data = (InputTextCallback_UserData*)data->UserData;
		if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
		{
			// Resize string callback
			// If for some reason we refuse the new length (BufTextLen) and/or capacity (BufSize) we need to set them back to what we want.
			std::string* str = user_data->Str;
			IM_ASSERT(data->Buf == str->c_str());
			str->resize(data->BufTextLen);
			data->Buf = (char*)str->c_str();
		}
		else if (user_data->ChainCallback)
		{
			// Forward to user callback, if any
			data->UserData = user_data->ChainCallbackUserData;
			return user_data->ChainCallback(data);
		}
		return 0;
	}

	bool InputText(const char* label, std::string* str, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback, void* user_data) {
		IM_ASSERT((flags & ImGuiInputTextFlags_CallbackResize) == 0);
		flags |= ImGuiInputTextFlags_CallbackResize;

		InputTextCallback_UserData cb_user_data;
		cb_user_data.Str = str;
		cb_user_data.ChainCallback = callback;
		cb_user_data.ChainCallbackUserData = user_data;
		return InputText(label, (char*)str->c_str(), str->capacity() + 1, flags, InputTextCallback, &cb_user_data);
	}

bool DrawInputFloat(float* data, const InputSettings* settings, int count) {
    assert(count > 0 && count < 5);
    assert(settings != nullptr);
    switch (count) {
        case 1:
            switch (settings->Type) {
                case ImGuiWidgetType::Input:
                    return ImGui::InputFloat("##InputFloat", data);
                case ImGuiWidgetType::Drag:
                    if (settings->UseRange)
                        return ImGui::DragFloat("##DragFloat", data, settings->Speed, settings->Min, settings->Max);
                    else
                        return ImGui::DragFloat("##DragFloat", data, settings->Speed);
                case ImGuiWidgetType::Slider:
                    return ImGui::SliderFloat("##SliderFloat", data, settings->Min, settings->Max);
                case ImGuiWidgetType::Color:
                    return false;
            }
            break;
        case 2:
            switch (settings->Type) {
                case ImGuiWidgetType::Input:
                    return ImGui::InputFloat2("##InputFloat2", data);
                case ImGuiWidgetType::Drag:
                    if (settings->UseRange)
                        return ImGui::DragFloat2("##DragFloat2", data, settings->Speed, settings->Min, settings->Max);
                    else
                        return ImGui::DragFloat2("##DragFloat2", data, settings->Speed);
                case ImGuiWidgetType::Slider:
                    return ImGui::SliderFloat2("##SliderFloat2", data, settings->Min, settings->Max);
                case ImGuiWidgetType::Color:
                    return false;
            }
            break;
        case 3:
            switch (settings->Type) {
                case ImGuiWidgetType::Input:
                    return ImGui::InputFloat3("##InputFloat3", data);
                case ImGuiWidgetType::Drag:
                    if (settings->UseRange)
                        return ImGui::DragFloat3("##DragFloat3", data, settings->Speed, settings->Min, settings->Max);
                    else
                        return ImGui::DragFloat3("##DragFloat3", data, settings->Speed);
                case ImGuiWidgetType::Slider:
                    return ImGui::SliderFloat3("##SliderFloat3", data, settings->Min, settings->Max);
                case ImGuiWidgetType::Color:
                    return ImGui::ColorEdit3("##ColorFloat3", data);
            }
            break;
        case 4:
            switch (settings->Type) {
                case ImGuiWidgetType::Input: {
                    return ImGui::InputFloat4("##InputFloat4", data);
                }
                case ImGuiWidgetType::Drag:
                    if (settings->UseRange)
                        return ImGui::DragFloat4("##DragFloat4", data, settings->Speed, settings->Min, settings->Max);
                    else
                        return ImGui::DragFloat4("##DragFloat4", data, settings->Speed);
                case ImGuiWidgetType::Slider:
                    return ImGui::SliderFloat4("##SliderFloat4", data, settings->Min, settings->Max);
                case ImGuiWidgetType::Color:
                    return ImGui::ColorEdit4("##ColorFloat4", data, ImGuiColorEditFlags_NoTooltip);
            }
            break;
        default:
            return false;
    }
}

bool DrawInputInt(int* data, const InputSettings* settings, int count) {
    assert(count > 0 && count < 5);
    assert(settings != nullptr);
    switch (count) {
        case 1:
            switch (settings->Type) {
                case ImGuiWidgetType::Input:
                    return ImGui::InputInt("##InputInt", data);
                case ImGuiWidgetType::Drag:
                    return ImGui::DragInt("##DragInt", data);
                case ImGuiWidgetType::Slider:
                    return ImGui::SliderInt("##SliderInt", data, settings->Min, settings->Max);
                case ImGuiWidgetType::Color:
                    return false;
            }
            break;
        case 2:
            switch (settings->Type) {
                case ImGuiWidgetType::Input:
                    return ImGui::InputInt2("##InputInt2", data);
                case ImGuiWidgetType::Drag:
                    return ImGui::DragInt2("##DragInt2", data);
                case ImGuiWidgetType::Slider:
                    return ImGui::SliderInt2("##SliderInt2", data, settings->Min, settings->Max);
                case ImGuiWidgetType::Color:
                    return false;
            }
            break;
        case 3:
            switch (settings->Type) {
                case ImGuiWidgetType::Input:
                    return ImGui::InputInt3("##InputInt3", data);
                case ImGuiWidgetType::Drag:
                    return ImGui::DragInt3("##DragInt3", data);
                case ImGuiWidgetType::Slider:
                    return ImGui::SliderInt3("##SliderInt3", data, settings->Min, settings->Max);
                case ImGuiWidgetType::Color:
                    return false;
            }
            break;
        case 4:
            switch (settings->Type) {
                case ImGuiWidgetType::Input:
                    return ImGui::InputInt4("##InputInt4", (int*) data);
                case ImGuiWidgetType::Drag:
                    return ImGui::DragInt4("##DragInt4", (int*) data);
                case ImGuiWidgetType::Slider:
                    return ImGui::SliderInt4("##SliderInt4", (int*) data, settings->Min, settings->Max);
                case ImGuiWidgetType::Color:
                    return false;
            }
            break;
        default:
            return false;
    }
}

bool DrawInputMat(float* data, const InputSettings* settings, int count) {
    assert(count > 2 && count < 5);
    assert(settings != nullptr);
    switch (count) {
        case 3:
            switch (settings->Type) {
                case ImGuiWidgetType::Input: {
                    bool changed1 = ImGui::InputFloat3("##InputMat3_1", data + 0);
                    bool changed2 = ImGui::InputFloat3("##InputMat3_2", data + 3);
                    bool changed3 = ImGui::InputFloat3("##InputMat3_3", data + 6);
                    return changed1 || changed2 || changed3;
                }
                case ImGuiWidgetType::Drag: {
                    if (settings->UseRange) {
                        bool changed1 = ImGui::DragFloat3("##DragMat3_1", data + 0, settings->Speed, settings->Min, settings->Max);
                        bool changed2 = ImGui::DragFloat3("##DragMat3_2", data + 3, settings->Speed, settings->Min, settings->Max);
                        bool changed3 = ImGui::DragFloat3("##DragMat3_3", data + 6, settings->Speed, settings->Min, settings->Max);
                        return changed1 || changed2 || changed3;
                    } else {
                        bool changed1 = ImGui::DragFloat3("##DragMat3_1", data + 0, settings->Speed);
                        bool changed2 = ImGui::DragFloat3("##DragMat3_2", data + 3, settings->Speed);
                        bool changed3 = ImGui::DragFloat3("##DragMat3_3", data + 6, settings->Speed);
                        return changed1 || changed2 || changed3;
                    }
                }
                case ImGuiWidgetType::Slider: {
                    bool changed1 = ImGui::SliderFloat3("##SliderMat3_1", data + 0, settings->Min, settings->Max);
                    bool changed2 = ImGui::SliderFloat3("##SliderMat3_2", data + 3, settings->Min, settings->Max);
                    bool changed3 = ImGui::SliderFloat3("##SliderMat3_3", data + 6, settings->Min, settings->Max);
                    return changed1 || changed2 || changed3;
                }
                case ImGuiWidgetType::Color: {
                    bool changed1 = ImGui::ColorEdit3("##ColorMat3_1", data + 0);
                    bool changed2 = ImGui::ColorEdit3("##ColorMat3_2", data + 3);
                    bool changed3 = ImGui::ColorEdit3("##ColorMat3_3", data + 6);
                    return changed1 || changed2 || changed3;
                }
            }
            break;
        case 4:
            switch (settings->Type) {
                case ImGuiWidgetType::Input: {
                    bool changed1 = ImGui::InputFloat4("##InputMat4_1", data + 0);
                    bool changed2 = ImGui::InputFloat4("##InputMat4_2", data + 4);
                    bool changed3 = ImGui::InputFloat4("##InputMat4_3", data + 8);
                    bool changed4 = ImGui::InputFloat4("##InputMat4_4", data + 12);
                    return changed1 || changed2 || changed3 || changed4;
                }
                case ImGuiWidgetType::Drag: {
                    if (settings->UseRange) {
                        bool changed1 = ImGui::DragFloat4("##DragMat4_1", data + 0, settings->Speed, settings->Min, settings->Max);
                        bool changed2 = ImGui::DragFloat4("##DragMat4_2", data + 4, settings->Speed, settings->Min, settings->Max);
                        bool changed3 = ImGui::DragFloat4("##DragMat4_3", data + 8, settings->Speed, settings->Min, settings->Max);
                        bool changed4 = ImGui::DragFloat4("##DragMat4_4", data + 12, settings->Speed, settings->Min, settings->Max);
                        return changed1 || changed2 || changed3 || changed4;
                    } else {
                        bool changed1 = ImGui::DragFloat4("##DragMat4_1", data + 0, settings->Speed);
                        bool changed2 = ImGui::DragFloat4("##DragMat4_2", data + 4, settings->Speed);
                        bool changed3 = ImGui::DragFloat4("##DragMat4_3", data + 8, settings->Speed);
                        bool changed4 = ImGui::DragFloat4("##DragMat4_4", data + 12, settings->Speed);
                        return changed1 || changed2 || changed3 || changed4;
                    }
                }
                case ImGuiWidgetType::Slider: {
                    bool changed1 = ImGui::SliderFloat4("##SliderMat4_1", data + 0, settings->Min, settings->Max);
                    bool changed2 = ImGui::SliderFloat4("##SliderMat4_2", data + 4, settings->Min, settings->Max);
                    bool changed3 = ImGui::SliderFloat4("##SliderMat4_3", data + 8, settings->Min, settings->Max);
                    bool changed4 = ImGui::SliderFloat4("##SliderMat4_4", data + 12, settings->Min, settings->Max);
                    return changed1 || changed2 || changed3 || changed4;
                }
                case ImGuiWidgetType::Color: {
                    bool changed1 = ImGui::ColorEdit3("##ColorMat4_1", data + 0);
                    bool changed2 = ImGui::ColorEdit3("##ColorMat4_2", data + 4);
                    bool changed3 = ImGui::ColorEdit3("##ColorMat4_3", data + 8);
                    bool changed4 = ImGui::ColorEdit3("##ColorMat4_4", data + 12);
                    return changed1 || changed2 || changed3 || changed4;
                }
            }
            break;
        default:
            return false;
    }
}

}
