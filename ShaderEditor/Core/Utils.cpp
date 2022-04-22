//
//  Utils.cpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 21.04.2022.
//

#include "Utils.hpp"
#include "Log.hpp"

#include <fstream>

namespace Engine {
namespace Utils {

std::optional<std::string> ReadFile(const std::string& filepath) {
    
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


}}
