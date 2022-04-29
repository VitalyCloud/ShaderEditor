//
//  Texture.hpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 29.04.2022.
//

#ifndef Texture_hpp
#define Texture_hpp

#include "Core/Core.hpp"
#include "glad/glad.h"

namespace OpenGL {

class Texture {
public:
    Texture(uint32_t width, uint32_t height);
    Texture(const std::string& filePath);
    ~Texture();
    
    uint32_t GetWidth() const { return m_Width; }
    uint32_t GetHeught() const { return m_Height; }
    
    void SetData(void* data, uint32_t size);
    void Bind(uint32_t slot = 0) const;
    bool IsLoaded() const { return m_IsLoaded; }
    bool operator==(const Texture& other) const {
        return m_RendererID == other.m_RendererID;
    }
    
    std::string& GetTitle() { return m_Title; }
    std::string& GetFilePath() { return m_Filepath; }
    
private:
    std::string m_Title;
    std::string m_Filepath;
    bool m_IsLoaded = false;
    uint32_t m_Width, m_Height;
    uint32_t m_RendererID;
    GLenum m_InternalFormat, m_DataFormat;
    
};


}




#endif /* Texture_hpp */
