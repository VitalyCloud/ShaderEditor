//
//  Log.cpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 22.04.2022.
//
#include "pch.h"
#include "Log.hpp"

#include "spdlog/sinks/stdout_color_sinks.h"

namespace Core {
    Ref<spdlog::logger> Log::s_CoreLogger;
    Ref<spdlog::logger> Log::s_ClientLogger;
    
    void Log::Init()
    {
        spdlog::set_pattern("%^[%T] %n: %v%$");
        
        s_CoreLogger = spdlog::stdout_color_mt("ENGINE");
        s_CoreLogger->set_level(spdlog::level::trace);
        
        s_ClientLogger = spdlog::stdout_color_mt("APP");
        s_ClientLogger->set_level(spdlog::level::trace);
    }
}
