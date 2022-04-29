//
//  Texture.cpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 29.04.2022.
//

#include "Core/pch.h"
#include "Texture.hpp"
#include "stb_image.h"

namespace OpenGL {

Texture::Texture(uint32_t width, uint32_t height) : m_Width(width), m_Height(height) {
    m_InternalFormat = GL_RGBA8;
    m_DataFormat = GL_RGBA;
    {
        glGenTextures(1, &m_RendererID);
        glBindTexture(GL_TEXTURE_2D, m_RendererID);

        glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat,
            m_Width, m_Height, 0, m_DataFormat, GL_UNSIGNED_BYTE, nullptr);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glBindTexture(GL_TEXTURE_2D, 0);
    }
}
Texture::Texture(const std::string& filePath) : m_Filepath(filePath) {
    int width, height, channels;
    stbi_set_flip_vertically_on_load(1);
    stbi_uc* data = nullptr;
    {
        data = stbi_load(filePath.c_str(), &width, &height, &channels, 0);
    }
    if (data)
    {
        m_IsLoaded = true;
        m_Width = width;
        m_Height = height;

        GLenum internalFormat = 0, dataFormat = 0;
        if (channels == 4) {
            internalFormat = GL_RGBA8;
            dataFormat = GL_RGBA;
        }
        else if (channels == 3) {
            internalFormat = GL_RGB8;
            dataFormat = GL_RGB;
        }

        m_InternalFormat = internalFormat;
        m_DataFormat = dataFormat;

        EN_CORE_ASSERT(internalFormat & dataFormat, "Format not supported!");
        {
            glGenTextures(1, &m_RendererID);
            glBindTexture(GL_TEXTURE_2D, m_RendererID);

            glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat,
                m_Width, m_Height, 0, m_DataFormat, GL_UNSIGNED_BYTE, data);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glTexParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

            glBindTexture(GL_TEXTURE_2D, 0);
        }
        stbi_image_free(data);
    }
}
Texture::~Texture() {
    glDeleteTextures(1, &m_RendererID);
}

void Texture::SetData(void* data, uint32_t size) {
    uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;
    EN_CORE_ASSERT(size == m_Width * m_Height * bpp, "Data must be entire texture!");
    glTexSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat,
        GL_UNSIGNED_BYTE, data);
}

void Texture::Bind(uint32_t slot) const {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_RendererID);
}

}
