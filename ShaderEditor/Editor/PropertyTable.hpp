//
//  PropertyTable.hpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 05.05.2022.
//

#ifndef PropertyTable_hpp
#define PropertyTable_hpp

#include "imgui.h"
#include "ImGuiHelper.hpp"

namespace Editor {

struct PropertyTable {
    
    static bool Begin(const char* title) {
        static bool tableFlags = ImGuiTableFlags_Resizable;
        bool returnValue = false;
        if(ImGui::BeginTable(title, 2, tableFlags)) {
            ImGui::TableSetupColumn("Property");
            ImGui::TableSetupColumn("Value");
            returnValue = true;
        }
        return returnValue;
    }

    static void End() {
        ImGui::EndTable();
    }

    template<typename T>
    static void Combo(const char* title, const char* valueNames[], int valuesSize, T& value) {
        ImGui::TableNextRow(); ImGui::TableNextColumn();
        ImGui::Text("%s", title);
        ImGui::TableNextColumn(); ImGui::PushItemWidth(-1);
        auto comboId = fmt::format("##{0}", title);
        int selected = static_cast<int>(value);
        if(ImGui::Combo(comboId.c_str(), &selected, valueNames, valuesSize))
            value = T(selected);
        ImGui::PopItemWidth();
    }

    static bool Text(const char* title, std::string& value) {
        bool returnValue;
        ImGui::TableNextRow(); ImGui::TableNextColumn();
        ImGui::Text("%s", title);
        ImGui::TableNextColumn(); ImGui::PushItemWidth(-1);
        auto inputTextId = fmt::format("##{0}", title);
        returnValue = ImGui::InputText(inputTextId.c_str(), &value);
        ImGui::PopItemWidth();
        return returnValue;
    }

    static bool Checkbox(const char* title, bool& value) {
        bool returnValue;
        ImGui::TableNextRow(); ImGui::TableNextColumn();
        ImGui::Text("%s", title);
        ImGui::TableNextColumn(); ImGui::PushItemWidth(-1);
        auto checkboxID = fmt::format("##{0}", title);
        returnValue = ImGui::Checkbox(checkboxID.c_str(), &value);
        ImGui::PopItemWidth();
        return returnValue;
    }

    static bool DragFloat(const char* title, float* value) {
        bool returnValue;
        ImGui::TableNextRow(); ImGui::TableNextColumn();
        ImGui::Text("%s", title);
        ImGui::TableNextColumn(); ImGui::PushItemWidth(-1);
        auto dragID = fmt::format("##{0}", title);
        returnValue =  ImGui::DragFloat(dragID.c_str(), value);
        ImGui::PopItemWidth();
        return returnValue;
    }

    static bool Color(const char* title, float* values, int size) {
        EN_ASSERT(size == 3 || size == 4);
        
        bool returnValue = false;
        ImGui::TableNextRow(); ImGui::TableNextColumn();
        ImGui::Text("%s", title);
        ImGui::TableNextColumn(); ImGui::PushItemWidth(-1);
        auto colorID = fmt::format("##{0}", title);
        
        if(size == 3) {
            returnValue = ImGui::ColorEdit3(colorID.c_str(), values);
        } else if (size == 4) {
            returnValue = ImGui::ColorEdit4(colorID.c_str(), values);
        }

        ImGui::PopItemWidth();
        return returnValue;
    }

    static bool InputUInt32(const char* title, unsigned int* value, int size) {
        EN_ASSERT(size > 0 && size < 5);
        
        bool returnValue = false;
        ImGui::TableNextRow(); ImGui::TableNextColumn();
        ImGui::Text("%s", title);
        ImGui::TableNextColumn(); ImGui::PushItemWidth(-1);
        auto inputID = fmt::format("##{0}", title);
        
        ImGui::InputScalarN(inputID.c_str(), ImGuiDataType_U32, value, size);

        ImGui::PopItemWidth();
        return returnValue;
    }
};

}



#endif /* PropertyTable_hpp */
