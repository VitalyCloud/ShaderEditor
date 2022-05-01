//
//  IndexBufferContainer.cpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 01.05.2022.
//
#include "pch.h"
#include "IndexBufferContainer.hpp"

namespace Editor {

IndexBufferContainer::IndexBufferContainer() {
    m_IndexBuffer = Core::CreateRef<OpenGL::IndexBuffer>(0);
}

IndexBufferContainer::~IndexBufferContainer() {
    
}

void IndexBufferContainer::UpdateIndexBufferIfNeeded() {
    if(m_State.CheckIf(IndexBufferState::SizeChanged)) {
        EN_INFO("Handle IB size change");
        m_IndexBuffer->Resize((uint32_t)m_Data.size());
        m_IndexBuffer->SetData(m_Data.data(), (uint32_t)m_Data.size());
        m_State.Reset();
        return;
    }
    
    if(m_State.CheckIf(IndexBufferState::DataChanged)) {
        EN_INFO("Handle IB data change");
        m_IndexBuffer->SetData(m_Data.data(), (uint32_t)m_Data.size());
        m_State.Reset();
        return;
    }
    
}

}
