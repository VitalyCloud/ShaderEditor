//
//  RenderState.cpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 02.05.2022.
//

#include "RenderState.hpp"

namespace OpenGL {

const char* POLYGON_MODE_NAMES[2] = {
    "Line",
    "Fill"
};

const char* CULL_MODE_NAMES[3] = {
    "Zero",
    "Front",
    "Back"
};

const char* CULL_FRONT_FACE_NAMES[2] = {
    "Clockwise",
    "Counter Clockwise"
};

const char* COMPARISON_FUNCTION_NAMES[8] = {
    "Never",
    "Less",
    "Equal",
    "LessEqual",
    "Greater",
    "NotEqual",
    "GreaterEqual",
    "Always"
};

const char* STENCIL_OPERATION_NAMES[8] = {
    "Keep",
    "Zero",
    "Replace",
    "Increase Saturated",
    "Decrease Saturated",
    "Invert",
    "Increase",
    "Decrease"
};

const char* BLEND_NAMES[13] = {
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
    "Saturated Source Alpha",
    "Blend Factor",
    "Inverse Blend Factor",
};

const char* BLEND_OPERATOR_NAMES[5] = {
    "Add",
    "Subtract",
    "Reverse Subtract",
    "Min",
    "Max"
};

static GLenum OpenGLCullMode(CullMode type) {
    switch(type) {
        case CullMode::None: return GL_ZERO;
        case CullMode::Front: return GL_FRONT;
        case CullMode::Back: return GL_BACK;
        default: EN_ASSERT(false,"Unknown CullMode type");
    }
    return -1;
}

static GLenum OpenGLPolygonMode(PolygonMode type) {
    switch (type) {
        case PolygonMode::Line: return GL_LINE;
        case PolygonMode::Fill: return GL_FILL;
        default: EN_ASSERT(false, "Unknown PolygonMode type");
    }
    return -1;
}

static GLenum OpenGLCullFrontFace(CullFrontFace type) {
    switch (type) {
        case CullFrontFace::ClockWise: return GL_CW;
        case CullFrontFace::CounterClockwise: return GL_CCW;
        default: EN_ASSERT(false, "Unknown CullFrontFace type");
    }
    return -1;
}

static GLenum OpenGLComparisonFunction(ComparisonFunction type) {
    switch(type) {
        case ComparisonFunction::Never: return GL_NEVER;
        case ComparisonFunction::Less: return GL_LESS;
        case ComparisonFunction::Equal: return GL_EQUAL;
        case ComparisonFunction::Lequal: return GL_LEQUAL;
        case ComparisonFunction::Greater: return GL_GREATER;
        case ComparisonFunction::NotEqual: return GL_NOTEQUAL;
        case ComparisonFunction::Gequal: return GL_GEQUAL;
        case ComparisonFunction::Always: return GL_ALWAYS;
        default: EN_ASSERT(false, "Unknown comparison function type");
    }
    return -1;
}

static GLenum OpenGLStencilOperation(StencilOperation type) {
    switch(type) {
        case StencilOperation::Keep: return GL_KEEP;
        case StencilOperation::Zero: return GL_ZERO;
        case StencilOperation::Replace: return GL_REPLACE;
        case StencilOperation::IncreaseSaturated: return GL_INCR_WRAP;
        case StencilOperation::DecreaseSaturated: return GL_DECR_WRAP;
        case StencilOperation::Invert: return GL_INVERT;
        case StencilOperation::Increase: return GL_INCR;
        case StencilOperation::Decrease: return GL_DECR;
        default: EN_ASSERT(false, "Unknown StencilOperation type");
    }
    return -1;
}

static GLenum OpenGLBlendFactor(BlendFactor type) {
    switch(type) {
        case BlendFactor::Zero: return GL_ZERO;
        case BlendFactor::One: return GL_ONE;
            
        case BlendFactor::SrcColor: return GL_SRC_COLOR;
        case BlendFactor::OneMinusSrcColor: return GL_ONE_MINUS_SRC_COLOR;
        case BlendFactor::SrcAlpha: return GL_SRC_ALPHA;
        case BlendFactor::OneMinusSrcAlpha: return GL_ONE_MINUS_SRC_ALPHA;
            
        case BlendFactor::DstColor: return GL_DST_COLOR;
        case BlendFactor::OneMinusDstColor: return GL_ONE_MINUS_DST_COLOR;
        case BlendFactor::DstAlpha: return GL_DST_ALPHA;
        case BlendFactor::OneMinusDstAlpha: return GL_ONE_MINUS_DST_ALPHA;
       
        case BlendFactor::SrcAlphaSaturate: return GL_SRC_ALPHA_SATURATE;
        case BlendFactor::ConstantColor: return GL_CONSTANT_COLOR;
        case BlendFactor::OneMinusConstatntColor: return GL_ONE_MINUS_CONSTANT_COLOR;
        default: EN_ASSERT(false, "Unknown BlendFactor type");
    }
    return -1;
}


static GLenum OpenGLBlendOperator(BlendOperator type) {
    switch(type) {
        case BlendOperator::Add: return GL_FUNC_ADD;
        case BlendOperator::Subtract: return GL_FUNC_SUBTRACT;
        case BlendOperator::ReverseSubtract: return GL_FUNC_REVERSE_SUBTRACT;
        case BlendOperator::Min: return GL_MIN;
        case BlendOperator::Max: return GL_MAX;
        default: EN_ASSERT(false, "Unknown BlendOperator type");
    }
    return -1;
}


RenderState::RenderState()
{
    PolygonMode = PolygonMode::Fill;
    CullFace = false;
    CullFaceType = CullMode::Back;
    CullFrontFace = CullFrontFace::CounterClockwise;

    Blend = false;
    AlphaToCoverage = false;
    BlendSourceFactorRGB = BlendFactor::SrcAlpha;
    BlendDestinationFactorRGB = BlendFactor::OneMinusSrcAlpha;
    BlendFunctionColor = BlendOperator::Add;
    BlendSourceFactorAlpha = BlendFactor::SrcAlpha;
    BlendDestinationFactorAlpha = BlendFactor::OneMinusSrcAlpha;
    BlendFunctionAlpha = BlendOperator::Add;
    BlendFactorColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

    DepthTest = true;
    DepthClamp = true;
    DepthMask = true;
    DepthFunction = ComparisonFunction::Less;
    DepthBias = 0.0f;

    StencilTest = false;
    StencilMask = 0x00;
    StencilFrontFaceFunction = StencilBackFaceFunction = ComparisonFunction::Equal;
    StencilReference = 0xFF;
    StencilFrontFaceOpPass = StencilFrontFaceOpDepthFail = StencilFrontFaceOpStencilFail = StencilOperation::Keep;
    StencilBackFaceOpPass = StencilBackFaceOpDepthFail = StencilBackFaceOpStencilFail = StencilOperation::Keep;
}

void RenderState::Bind() {
    // depth clamp
    if (DepthClamp)
        glEnable(GL_DEPTH_CLAMP);
    else
        glDisable(GL_DEPTH_CLAMP);

    // fill mode
    glPolygonMode(GL_FRONT_AND_BACK, OpenGLPolygonMode(PolygonMode));

    // culling and front face
    if (CullFace)
        glEnable(GL_CULL_FACE);
    else
        glDisable(GL_CULL_FACE);
    glCullFace(OpenGLCullMode(CullFaceType));
    glFrontFace(OpenGLCullFrontFace(CullFrontFace));

    // disable blending
    if (Blend) {
        glEnable(GL_BLEND);
        glBlendEquationSeparate(OpenGLBlendOperator(BlendFunctionColor), OpenGLBlendOperator(BlendFunctionAlpha));
        glBlendFuncSeparate(OpenGLBlendFactor(BlendSourceFactorRGB), OpenGLBlendFactor(BlendDestinationFactorRGB), OpenGLBlendFactor(BlendSourceFactorAlpha), OpenGLBlendFactor(BlendDestinationFactorAlpha));
        glBlendColor(BlendFactorColor.r, BlendFactorColor.g, BlendFactorColor.a, BlendFactorColor.a);
        glSampleCoverage(AlphaToCoverage, GL_FALSE);
    } else
        glDisable(GL_BLEND);

    // depth state
    if (DepthTest)
        glEnable(GL_DEPTH_TEST);
    else
        glDisable(GL_DEPTH_TEST);
    glDepthMask(DepthMask);
    glDepthFunc(OpenGLComparisonFunction(DepthFunction));
    glPolygonOffset(0.0f, DepthBias);

    // stencil
    if (StencilTest) {
        glEnable(GL_STENCIL_TEST);
        glStencilFuncSeparate(GL_FRONT, OpenGLComparisonFunction(StencilFrontFaceFunction), 1, StencilReference);
        glStencilFuncSeparate(GL_BACK, OpenGLComparisonFunction(StencilBackFaceFunction), 1, StencilReference);
        glStencilMask(StencilMask);
        glStencilOpSeparate(GL_FRONT,
                            OpenGLStencilOperation(StencilFrontFaceOpStencilFail),
                            OpenGLStencilOperation(StencilFrontFaceOpDepthFail),
                            OpenGLStencilOperation(StencilFrontFaceOpPass));
        glStencilOpSeparate(GL_BACK,
                            OpenGLStencilOperation(StencilBackFaceOpStencilFail),
                            OpenGLStencilOperation(StencilBackFaceOpDepthFail),
                            OpenGLStencilOperation(StencilBackFaceOpPass));
    } else
        glDisable(GL_STENCIL_TEST);
}

void RenderState::BindDefault() {
    // render states
    glDisable(GL_DEPTH_CLAMP);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
    glDisable(GL_CULL_FACE);
    
    // disable blending
    glDisable(GL_BLEND);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    // depth state
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);

    // stencil
    glDisable(GL_STENCIL_TEST);
}

}
