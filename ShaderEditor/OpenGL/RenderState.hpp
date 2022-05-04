//
//  RenderState.hpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 02.05.2022.
//

#ifndef RenderState_hpp
#define RenderState_hpp

#include "glad/glad.h"
#include "glm/glm.hpp"

namespace OpenGL {

extern const char* POLYGON_MODE_NAMES[2];
extern const char* CULL_MODE_NAMES[3];
extern const char* CULL_FRONT_FACE_NAMES[2];
extern const char* COMPARISON_FUNCTION_NAMES[8];
extern const char* STENCIL_OPERATION_NAMES[8];
extern const char* BLEND_NAMES[13];
extern const char* BLEND_OPERATOR_NAMES[5];

enum class ComparisonFunction {
    Never = 0,
    Less,
    Equal,
    Lequal,
    Greater,
    NotEqual,
    Gequal,
    Always
};

enum class StencilOperation {
    Keep = 0,
    Zero,
    Replace,
    IncreaseSaturated,
    DecreaseSaturated,
    Invert,
    Increase,
    Decrease
};

enum class PolygonMode {
    Line = 0,
    Fill
};

enum class CullMode {
    None = 0,
    Front,
    Back
};

enum class CullFrontFace {
    ClockWise = 0,
    CounterClockwise
};

enum class BlendFactor {
    Zero = 0,
    One,
    
    SrcColor,
    OneMinusSrcColor,
    SrcAlpha,
    OneMinusSrcAlpha,
    
    DstColor,
    OneMinusDstColor,
    DstAlpha,
    OneMinusDstAlpha,
    
    SrcAlphaSaturate,
    ConstantColor,
    OneMinusConstatntColor
};

enum class BlendOperator {
    Add = 0,
    Subtract,
    ReverseSubtract,
    Min,
    Max
};

struct RenderState {
    PolygonMode PolygonMode;
    bool CullFace;
    CullMode CullFaceType;
    CullFrontFace CullFrontFace;

    bool Blend;
    bool AlphaToCoverage;
    BlendFactor BlendSourceFactorRGB;
    BlendFactor BlendDestinationFactorRGB;
    BlendOperator BlendFunctionColor;
    BlendFactor BlendSourceFactorAlpha;
    BlendFactor BlendDestinationFactorAlpha;
    BlendOperator BlendFunctionAlpha;
    glm::vec4 BlendFactorColor;

    bool DepthTest;
    bool DepthClamp;
    bool DepthMask;
    ComparisonFunction DepthFunction;
    float DepthBias;

    bool StencilTest;
    unsigned int StencilMask;
    ComparisonFunction StencilFrontFaceFunction, StencilBackFaceFunction;
    unsigned int StencilReference;
    StencilOperation StencilFrontFaceOpStencilFail, StencilFrontFaceOpDepthFail, StencilFrontFaceOpPass;
    StencilOperation StencilBackFaceOpStencilFail, StencilBackFaceOpDepthFail, StencilBackFaceOpPass;

    RenderState();
    void Bind();
    static void BindDefault();
};

}



#endif /* RenderState_hpp */
