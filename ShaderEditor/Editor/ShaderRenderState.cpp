//
//  ShaderRenderState.cpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 04.05.2022.
//
#include "pch.h"
#include "ShaderRenderState.hpp"
#include "PropertyTable.hpp"

#include "imgui.h"
#include "glm/gtc/type_ptr.hpp"

namespace Editor {
    void ShaderRenderStateInspector::Draw() {
        if(m_Context == nullptr) { return; }
        ImGui::Text("Render State");
        
        ImGui::Separator();
        if(PropertyTable::Begin("ShaderRenderStatePropertiesTable")) {
            ImGui::PushID("RenderState");
            
            PropertyTable::Text("Title", m_Context->m_Title);
            ImGui::Separator();
            PropertyTable::Combo("Polygon Mode", OpenGL::POLYGON_MODE_NAMES,
                               IM_ARRAYSIZE(OpenGL::POLYGON_MODE_NAMES), m_Context->m_RenderState.PolygonMode);
            
            ImGui::Separator();
            PropertyTable::Checkbox("Cull", m_Context->m_RenderState.CullFace);
            
            if(m_Context->m_RenderState.CullFace) {
                PropertyTable::Combo("Cull mode", OpenGL::CULL_MODE_NAMES,
                                   IM_ARRAYSIZE(OpenGL::CULL_MODE_NAMES), m_Context->m_RenderState.CullFaceType);
                PropertyTable::Combo("Cull front face", OpenGL::CULL_FRONT_FACE_NAMES,
                                   IM_ARRAYSIZE(OpenGL::CULL_FRONT_FACE_NAMES), m_Context->m_RenderState.CullFrontFace);
            }
            
            ImGui::Separator();
            PropertyTable::Checkbox("Depth Test", m_Context->m_RenderState.DepthTest);
            
            if(m_Context->m_RenderState.DepthTest) {
                PropertyTable::Checkbox("Depth Clamp", m_Context->m_RenderState.DepthClamp);
                PropertyTable::Checkbox("Depth Mask", m_Context->m_RenderState.DepthMask);
                PropertyTable::Combo("Depth function", OpenGL::COMPARISON_FUNCTION_NAMES,
                                   IM_ARRAYSIZE(OpenGL::COMPARISON_FUNCTION_NAMES), m_Context->m_RenderState.DepthFunction);
                PropertyTable::DragFloat("Depth bias", &m_Context->m_RenderState.DepthBias);
            }
            
            ImGui::Separator();
            PropertyTable::Checkbox("Blending", m_Context->m_RenderState.Blend);
            
            if(m_Context->m_RenderState.Blend) {
                PropertyTable::Checkbox("Alpha to coverege", m_Context->m_RenderState.AlphaToCoverage);
                PropertyTable::Combo("Source blend factor", OpenGL::BLEND_NAMES,
                                   IM_ARRAYSIZE(OpenGL::BLEND_NAMES), m_Context->m_RenderState.BlendSourceFactorRGB);
                
                PropertyTable::Combo("Blend Operator", OpenGL::BLEND_OPERATOR_NAMES,
                                   IM_ARRAYSIZE(OpenGL::BLEND_OPERATOR_NAMES), m_Context->m_RenderState.BlendFunctionColor);
                
                PropertyTable::Combo("Destination blend factor", OpenGL::BLEND_NAMES,
                                   IM_ARRAYSIZE(OpenGL::BLEND_NAMES), m_Context->m_RenderState.BlendDestinationFactorRGB);
                
                PropertyTable::Combo("Source alpha blend factor", OpenGL::BLEND_NAMES,
                                   IM_ARRAYSIZE(OpenGL::BLEND_NAMES), m_Context->m_RenderState.BlendSourceFactorAlpha);
                
                PropertyTable::Combo("Alpha Blend Operator", OpenGL::BLEND_OPERATOR_NAMES,
                                   IM_ARRAYSIZE(OpenGL::BLEND_OPERATOR_NAMES), m_Context->m_RenderState.BlendFunctionAlpha);
                
                PropertyTable::Combo("Destination alpha blend factor", OpenGL::BLEND_NAMES,
                                   IM_ARRAYSIZE(OpenGL::BLEND_NAMES), m_Context->m_RenderState.BlendDestinationFactorAlpha);
                
                PropertyTable::Color("Custom blend factor",
                                   glm::value_ptr(m_Context->m_RenderState.BlendFactorColor), 4);
            }
            
            ImGui::Separator();
            PropertyTable::Checkbox("Stencil Test", m_Context->m_RenderState.StencilTest);
            
            if(m_Context->m_RenderState.StencilTest) {
                PropertyTable::InputUInt32("Stencil mask", &m_Context->m_RenderState.StencilMask, 1);
                PropertyTable::InputUInt32("Stencil reference", &m_Context->m_RenderState.StencilReference, 1);
                
                PropertyTable::Combo("Stencil front face function", OpenGL::COMPARISON_FUNCTION_NAMES,
                                   IM_ARRAYSIZE(OpenGL::COMPARISON_FUNCTION_NAMES), m_Context->m_RenderState.StencilFrontFaceFunction);
                
                PropertyTable::Combo("Stencil front face pass", OpenGL::STENCIL_OPERATION_NAMES,
                                   IM_ARRAYSIZE(OpenGL::STENCIL_OPERATION_NAMES), m_Context->m_RenderState.StencilFrontFaceOpPass);
                
                PropertyTable::Combo("Stencil front face fail", OpenGL::STENCIL_OPERATION_NAMES,
                                   IM_ARRAYSIZE(OpenGL::STENCIL_OPERATION_NAMES), m_Context->m_RenderState.StencilFrontFaceOpStencilFail);
                
                PropertyTable::Combo("Depth front face fail",OpenGL::STENCIL_OPERATION_NAMES,
                                   IM_ARRAYSIZE(OpenGL::STENCIL_OPERATION_NAMES), m_Context->m_RenderState.StencilFrontFaceOpDepthFail);
                
                
                PropertyTable::Combo("Stencil back face function", OpenGL::COMPARISON_FUNCTION_NAMES,
                                   IM_ARRAYSIZE(OpenGL::COMPARISON_FUNCTION_NAMES), m_Context->m_RenderState.StencilBackFaceFunction);
                
                PropertyTable::Combo("Stencil back face pass", OpenGL::STENCIL_OPERATION_NAMES,
                                   IM_ARRAYSIZE(OpenGL::STENCIL_OPERATION_NAMES), m_Context->m_RenderState.StencilBackFaceOpPass);
                
                PropertyTable::Combo("Stencil back face fail", OpenGL::STENCIL_OPERATION_NAMES,
                                   IM_ARRAYSIZE(OpenGL::STENCIL_OPERATION_NAMES), m_Context->m_RenderState.StencilBackFaceOpStencilFail);
                
                PropertyTable::Combo("Depth back face fail",OpenGL::STENCIL_OPERATION_NAMES,
                                   IM_ARRAYSIZE(OpenGL::STENCIL_OPERATION_NAMES), m_Context->m_RenderState.StencilBackFaceOpDepthFail);
                
            }
            ImGui::PopID();
            PropertyTable::End();
        }
        ImGui::Separator();
    }
}
