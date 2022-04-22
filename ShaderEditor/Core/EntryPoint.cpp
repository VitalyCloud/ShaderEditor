//
//  main.cpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 20.04.2022.
//
#include "Core/pch.h"
#include "Application.hpp"


extern Core::Application* CreateApplication(int argc, char** argv);

int main(int argc, char * argv[]) {
    Core::Log::Init();
    Core::Application* app = Core::CreateApplication(argc, argv);
    app->Run();
    delete app;
    
    return 0;
}
