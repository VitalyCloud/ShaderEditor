//
//  Log.hpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 22.04.2022.
//

#ifndef Log_hpp
#define Log_hpp

#include "spdlog/spdlog.h"
#include "Memory.hpp"

namespace Core {
    class Log
    {
    public:
        static void Init();
        
        inline static Ref<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
        inline static Ref<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
    private:
        // Initialize in EntryPoint main() function
        static Ref<spdlog::logger> s_CoreLogger;
        static Ref<spdlog::logger> s_ClientLogger;
    };
}

#define EN_LOG_ENABLE

#ifdef EN_LOG_ENABLE

#define EN_CORE_TRACE(...)     ::Core::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define EN_CORE_INFO(...)      ::Core::Log::GetCoreLogger()->info(__VA_ARGS__)
#define EN_CORE_WARN(...)      ::Core::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define EN_CORE_ERROR(...)     ::Core::Log::GetCoreLogger()->error(__VA_ARGS__)
#define EN_CORE_CRITICAL(...)  ::Core::Log::GetCoreLogger()->critical(__VA_ARGS__)

#define EN_TRACE(...)          ::Core::Log::GetClientLogger()->trace(__VA_ARGS__)
#define EN_INFO(...)           ::Core::Log::GetClientLogger()->info(__VA_ARGS__)
#define EN_WARN(...)           ::Core::Log::GetClientLogger()->warn(__VA_ARGS__)
#define EN_ERROR(...)          ::Core::Log::GetClientLogger()->error(__VA_ARGS__)
#define EN_CRITICAL(...)       ::Core::Log::GetClientLogger()->critical(__VA_ARGS__)

#else
    #define EN_CORE_TRACE(...)
    #define EN_CORE_INFO(...)
    #define EN_CORE_WARN(...)
    #define EN_CORE_ERROR(...)
    #define EN_CORE_CRITICAL(...)

    #define EN_TRACE(...)
    #define EN_INFO(...)
    #define EN_WARN(...)
    #define EN_ERROR(...)
    #define EN_CRITICAL(...)
#endif

#endif /* Log_hpp */
