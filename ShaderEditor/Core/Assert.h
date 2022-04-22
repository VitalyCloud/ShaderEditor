//
//  Assert.h
//  ShaderEditor
//
//  Created by Vitaly Cloud on 22.04.2022.
//

#ifndef Assert_h
#define Assert_h

#include <filesystem>
#include "Log.hpp"

#include "PlatformDetection.h"

#define EN_EXPAND_MACRO(x) x
#define EN_STRINGIFY_MACRO(x) #x

#ifdef EN_DEBUG
    #if defined(EN_PLATFORM_WINDOWS)
        #define EN_DEBUGBREAK() __debugbreak()
    #elif defined(EN_PLATFORM_LINUX)
        #include <signal.h>
        #define EN_DEBUGBREAK() raise(SIGTRAP)
    #elif defined(EN_PLATFORM_MACOSX)
        #define EN_DEBUGBREAK() __builtin_trap()
    #else
        #error "Platform doesn't support debugbreak yet!"
    #endif
    #define EN_ENABLE_ASSERTS
#else
    #define EN_DEBUGBREAK()
#endif

//#ifdef EN_ENABLE_ASSERTS
//    #define EN_INTERNAL_ASSERT_IMPL(type, check, msg, ...) { if(!(check)) { EN_ERROR(msg, __VA_ARGS__); EN_DEBUGBREAK(); } }
//    #define EN_INTERNAL_ASSERT_WITH_MSG(type, check, ...) EN_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
//    #define EN_INTERNAL_ASSERT_NO_MSG(type, check) EN_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", EN_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)
//
//    #define EN_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
//    #define EN_INTERNAL_ASSERT_GET_MACRO(...) EN_EXPAND_MACRO( EN_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, EN_INTERNAL_ASSERT_WITH_MSG, EN_INTERNAL_ASSERT_NO_MSG) )
//
//    #define EN_ASSERT(...) EN_EXPAND_MACRO( EN_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__) )
//    #define EN_CORE_ASSERT(...) EN_EXPAND_MACRO( EN_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__) )
//#else
//    #define EN_ASSERT(...)
//    #define EN_CORE_ASSERT(...)
//#endif
#ifdef EN_ENABLE_ASSERTS
    #define EN_INTERNAL_ASSERT_IMPL(type, check, msg, ...) { if(!(check)) { EN##type##ERROR(msg, __VA_ARGS__); EN_DEBUGBREAK(); } }
    #define EN_INTERNAL_ASSERT_WITH_MSG(type, check, ...) EN_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
    #define EN_INTERNAL_ASSERT_NO_MSG(type, check) EN_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", EN_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)

    #define EN_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
    #define EN_INTERNAL_ASSERT_GET_MACRO(...) EN_EXPAND_MACRO( EN_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, EN_INTERNAL_ASSERT_WITH_MSG, EN_INTERNAL_ASSERT_NO_MSG) )

    #define EN_ASSERT(...) EN_EXPAND_MACRO( EN_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__) )
    #define EN_CORE_ASSERT(...) EN_EXPAND_MACRO( EN_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__) )
#else
    #define EN_ASSERT(...)
    #define EN_CORE_ASSERT(...)
#endif


#endif /* Assert_h */
