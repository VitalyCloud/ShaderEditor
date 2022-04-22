//
//  Utils.cpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 21.04.2022.
//
#include "Core/pch.h"

namespace Engine {
namespace Utils {


std::optional<std::string> FileSystem::ReadFile(const std::string& filepath) {
    
    std::string result;
    // ifstream closes itself due to RAII
    std::ifstream in(filepath, std::ios::in | std::ios::binary);
    
    if(in) {
        in.seekg(0, std::ios::end);
        size_t size = in.tellg();
        
        if(size != -1) {
            result.resize(size);
            in.seekg(0, std::ios::beg);
            in.read(&result[0], size);
        } else {
            EN_CORE_ERROR("Could not read from file: {0}", filepath);
            return {};
        }
    } else {
        EN_CORE_ERROR("Could not open file: {0}", filepath);
        return  {};
    }
    
    return result;
}


void FileSystem::WriteFile(const std::string& content, const std::string& filepath) {
    std::ofstream out(filepath);
    if(out.good()) {
        out << content;
        EN_INFO("File '{0}' saved", filepath);
    } else {
        EN_ERROR("File '{0}' is not saved", filepath);
    }
    out.close();
}


}}
