//
//  main.cpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 20.04.2022.
//

#include "Application.hpp"
#include "Log.hpp"

extern Engine::Application* CreateApplication(int argc, char** argv);

int main(int argc, char * argv[]) {
    Engine::Log::Init();
    Engine::Application* app = Engine::CreateApplication(argc, argv);
    app->Run();
    delete app;
    
    return 0;
}
