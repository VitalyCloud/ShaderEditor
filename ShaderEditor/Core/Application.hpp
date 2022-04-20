//
//  Application.hpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 20.04.2022.
//

#ifndef Application_hpp
#define Application_hpp

#include "Layer.h"

#include <string>
#include <memory>
#include <vector>

struct GLFWwindow;

namespace Editor {

struct ApplicationSpecification {
    std::string Name = "Application";
    uint32_t Width = 1600;
    uint32_t Height = 900;
};

class Application {
public:
    Application(const ApplicationSpecification& applicationSpecification = ApplicationSpecification());
    
    ~Application();
    
    void Run();
    void PushLayer(const std::shared_ptr<Layer>& layer) { m_LayerStack.emplace_back(layer); layer->OnAttach(); }
    
    template<typename T>
    void PushLayer() {
        static_assert(std::is_base_of<Layer, T>::value, "Pushed type is not subclass of Layer!");
        m_LayerStack.emplace_back(std::make_shared<T>())->OnAttach();
    }
    
    void Close();
    
private:
    void Init();
    void Shutdown();
    
private:
    ApplicationSpecification m_Specification;
    GLFWwindow* m_WindowHandle = nullptr;
    bool m_Running = false;
    
    using LayerStack = std::vector<std::shared_ptr<Layer>>;
    LayerStack m_LayerStack;
};

// Implemented by CLIENT
Application* CreateApplication(int argc, char** argv);

}


#endif /* Application_hpp */
