//
//  Names.hpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 04.05.2022.
//

#ifndef Names_hpp
#define Names_hpp

#include <stdio.h>
#include "glad/glad.h"

const char* BLEND_NAMES[] = {
    "",
    "Zero",
    "One",
    "Source Color",
    "Inverse Source Color",
    "Source Alpha",
    "Inverse Source Alpha",
    "Destination Alpha",
    "Inverse Destination Alpha",
    "Destination Color",
    "Inverse Destination Color",
    "Saturated Source Alpha", // 11
    "Blend Factor",
    "Inverse Blend Factor",
};

const unsigned int BLEND_VALUES[] = {
    static_cast<unsigned int>(-1),
    GL_ZERO,
    GL_ONE,
    GL_SRC_COLOR,
    GL_ONE_MINUS_SRC_COLOR,
    GL_SRC_ALPHA,
    GL_ONE_MINUS_SRC_ALPHA,
    GL_DST_ALPHA,
    GL_ONE_MINUS_DST_ALPHA,
    GL_DST_COLOR,
    GL_ONE_MINUS_DST_COLOR,
    GL_SRC_ALPHA_SATURATE, // 11
    GL_CONSTANT_COLOR,
    GL_ONE_MINUS_CONSTANT_COLOR,
};

const char* BLEND_OPERATOR_NAMES[] = {
    "",
    "Add",
    "Subtract",
    "Reverse Subtract",
    "Min",
    "Max"
};

const unsigned int BLEND_OPERATOR_VALUES[] = {
    static_cast<unsigned int>(-1),
    GL_FUNC_ADD,
    GL_FUNC_SUBTRACT,
    GL_FUNC_REVERSE_SUBTRACT,
    GL_MIN,
    GL_MAX
};

const char* COMPARISON_FUNCTION_NAMES[] = {
    "",
    "Never",
    "Less",
    "Equal",
    "LessEqual",
    "Greater",
    "NotEqual",
    "GreaterEqual",
    "Always"
};

const unsigned int COMPARISON_FUNCTION_VALUES[] = {
    static_cast<unsigned int>(-1),
    GL_NEVER,
    GL_LESS,
    GL_EQUAL,
    GL_LEQUAL,
    GL_GREATER,
    GL_NOTEQUAL,
    GL_GEQUAL,
    GL_ALWAYS
};

const char* STENCIL_OPERATION_NAMES[] = {
    "",
    "Keep",
    "Zero",
    "Replace",
    "Increase Saturated",
    "Decrease Saturated",
    "Invert",
    "Increase",
    "Decrease"
};

const unsigned int STENCIL_OPERATION_VALUES[] = {
    GL_ZERO,
    GL_KEEP,
    GL_ZERO,
    GL_REPLACE,
    GL_INCR_WRAP,
    GL_DECR_WRAP,
    GL_INVERT,
    GL_INCR,
    GL_DECR
};



#endif /* Names_hpp */
