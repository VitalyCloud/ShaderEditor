//
//  Application.hpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 20.04.2022.
//

#ifndef Application_hpp
#define Application_hpp

#include "Layer.hpp"
#include "Events/Event.hpp"
#include "Events/ApplicationEvent.hpp"

#include <string>
#include <memory>
#include <vector>

struct GLFWwindow;

namespace Engine {

struct ApplicationSpecification {
    std::string Name = "Application";
    uint32_t Width = 1600;
    uint32_t Height = 900;
};

class Application {
public:
    Application(const ApplicationSpecification& applicationSpecification = ApplicationSpecification());
    
    ~Application();
    
    void OnEvent(Event &e);
    void Run();
    void PushLayer(const std::shared_ptr<Layer>& layer) { m_LayerStack.emplace_back(layer); layer->OnAttach(); }
    
    template<typename T>
    void PushLayer() {
        static_assert(std::is_base_of<Layer, T>::value, "Pushed type is not subclass of Layer!");
        m_LayerStack.emplace_back(std::make_shared<T>())->OnAttach();
    }
    
    void Close();
    
    GLFWwindow* GetWindow() { return m_WindowHandle; }
    static Application& Get() { return *s_Application; }
    
private:
    void Init();
    void Shutdown();
    void SetCallbacks();
    bool OnWindowClose(WindowCloseEvent& e);
    bool OnWindowResize(WindowResizeEvent& e);
    
private:
    static Application* s_Application;
    using EventCallbackFn = std::function<void(Engine::Event&)>;
    struct WindowData
    {
        std::string Title;
        int Width, Height;
        bool VSync;
        
        EventCallbackFn EventCallback;
    };
    WindowData m_Data;
    
    GLFWwindow* m_WindowHandle = nullptr;
    bool m_Running = false;
    
    using LayerStack = std::vector<std::shared_ptr<Layer>>;
    LayerStack m_LayerStack;
};

// Implemented by CLIENT
Application* CreateApplication(int argc, char** argv);

}


#endif /* Application_hpp */
