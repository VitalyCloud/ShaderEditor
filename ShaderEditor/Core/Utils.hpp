//
//  Utils.hpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 21.04.2022.
//

#ifndef Utils_hpp
#define Utils_hpp

#include <optional>

namespace Core {
namespace Utils {


class FileDialogs
{
public:
    // To be difined by Platforms
    static std::string OpenFile(const char* filter);
    static std::string SaveFile(const char* filter);
};


class FileSystem
{
public:
    static std::optional<std::string> ReadFile(const std::string& filepath);
    static void WriteFile(const std::string& content, const std::string& filepath);
};

}}

#endif /* Utils_hpp */
