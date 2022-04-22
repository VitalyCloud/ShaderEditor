//
//  Application.cpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 20.04.2022.
//
#include "Core/pch.h"
#include "Application.hpp"
#include "Events/ApplicationEvent.hpp"
#include "Events/KeyEvent.hpp"
#include "Events/MouseEvent.hpp"
#include "Renderer/Renderer.hpp"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"


#include "ImGui/Roboto-Regular.embed"

static void glfw_error_callback(int error, const char* description)
{
    EN_CORE_ERROR("GLFW Error {0}: {1}", error, description);
}

namespace Engine {

#define BIND_EVENT_FN(x) std::bind(x, this, std::placeholders::_1)

Application* Application::s_Application = nullptr;

Application::Application(const ApplicationSpecification& specification){
    EN_CORE_ASSERT(s_Application == nullptr, "Application is already created!");
    s_Application = this;
    m_Data.Title = specification.Name;
    m_Data.Width = specification.Width;
    m_Data.Height = specification.Height;
    m_Data.EventCallback = BIND_EVENT_FN(&Application::OnEvent);
    Init();
}

Application::~Application() {
    Shutdown();
}

void Application::Init() {
    glfwSetErrorCallback(glfw_error_callback);
    
    EN_CORE_ASSERT(glfwInit(), "Could not initializa GLFW!");
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef EN_PLATFORM_MACOSX
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    m_WindowHandle = glfwCreateWindow(m_Data.Width, m_Data.Height, m_Data.Title.c_str(), NULL, NULL);
    
    glfwMakeContextCurrent(m_WindowHandle);
    
    SetVSync(true);
    
    EN_INFO("Application {0} created. Window size: {1}x{2}. VSync is {3}", m_Data.Title, m_Data.Width, m_Data.Height, m_Data.VSync);
    
    EN_CORE_ASSERT(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress), "Failed to initialize GLAD");
    
    glfwSetWindowUserPointer(m_WindowHandle, &m_Data);
    SetCallbacks();
    
    const char* glsl_version = "#version 410";
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void) io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    
    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }
    
    ImGui_ImplGlfw_InitForOpenGL(m_WindowHandle, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    
    // Load default font
    ImFontConfig fontConfig;
    fontConfig.FontDataOwnedByAtlas = false;
    ImFont* robotoFont = io.Fonts->AddFontFromMemoryTTF((void*)g_RobotoRegular, sizeof(g_RobotoRegular), 18.0f, &fontConfig);
    io.FontDefault = robotoFont;
}

void Application::Close() {
    m_Running = false;
}

void Application::SetVSync(bool enabled) {
    glfwSwapInterval(enabled ? 1 : 0);
    m_Data.VSync = enabled;
}

void Application::OnEvent(Event &e) {
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(&Application::OnWindowClose));
    dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(&Application::OnWindowResize));

    for(auto it = m_LayerStack.end(); it!=m_LayerStack.begin(); )
    {
        (*--it)->OnEvent(e);
        if(e.Handled)
            break;
    }
}

bool Application::OnWindowClose(WindowCloseEvent& e) {
    Close();
    return true;
}

bool Application::OnWindowResize(WindowResizeEvent& e) {
    Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());
    return false;
}

void Application::Shutdown() {
    
    for(auto& layer : m_LayerStack)
        layer->OnDetach();
    
    m_LayerStack.clear();
    
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    
    glfwDestroyWindow(m_WindowHandle);
    glfwTerminate();
}

void Application::Run() {
    while(!glfwWindowShouldClose(m_WindowHandle) && m_Running) {
        
        glfwSwapBuffers(m_WindowHandle);
        glfwPollEvents();
        // MARK: - Rendering
        
        for(auto& layer: m_LayerStack) {
            layer->OnUpdate();
        }
        
        
        // MARK: - ImGui Rendering
        
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        {
            // Docking
            // Note: Switch this to true to enable dockspace
            static bool dockspaceOpen = true;
            static bool opt_fullscreen_persistant = true;
            bool opt_fullscreen = opt_fullscreen_persistant;
            static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
            //static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_AutoHideTabBar;


            // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
            // because it would be confusing to have two docking targets within each others.
            ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
            if(m_MenubarCallback)
                window_flags |= ImGuiWindowFlags_MenuBar;
            
            if (opt_fullscreen)
            {
                ImGuiViewport* viewport = ImGui::GetMainViewport();
                ImGui::SetNextWindowPos(viewport->Pos);
                ImGui::SetNextWindowSize(viewport->Size);
                ImGui::SetNextWindowViewport(viewport->ID);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
                window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
                window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
            }

            // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
            if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
                window_flags |= ImGuiWindowFlags_NoBackground;

            // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
            // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
            // all active windows docked into it will lose their parent and become undocked.
            // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
            // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
            ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);

            ImGui::PopStyleVar();
            if (opt_fullscreen)
                ImGui::PopStyleVar(2);

            // DockSpace
            ImGuiIO& io = ImGui::GetIO();
            ImGuiStyle& style = ImGui::GetStyle();
            float minWinSizeX = style.WindowMinSize.x;
            style.WindowMinSize.x = 370.0f;
            if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
            {
                ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
                ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
            }
            style.WindowMinSize.x = minWinSizeX;
            
            if (m_MenubarCallback)
            {
                if (ImGui::BeginMainMenuBar())
                {
                    m_MenubarCallback();
                    ImGui::EndMainMenuBar();
                }
            }
            
            // Rendering menus
            for(auto& layer: m_LayerStack) {
                layer->OnUIRender();
            }
            
            ImGui::End();
        }
        
        // End Frame
        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize = ImVec2(m_Data.Width, m_Data.Height);
        // Rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
        ImGui::EndFrame();
    }
}

void Application::SetCallbacks() {
    glfwSetFramebufferSizeCallback(m_WindowHandle, [](GLFWwindow* window, int width, int height) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        WindowResizeEvent event(width, height);
        data.Width = width;
        data.Height = height;
        data.EventCallback(event);
    });
    
    glfwSetWindowCloseCallback(m_WindowHandle, [](GLFWwindow* window)
    {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        WindowCloseEvent event;
        data.EventCallback(event);
    });
    
    glfwSetKeyCallback(m_WindowHandle, [](GLFWwindow* window,
                        int key, int scancode,
                        int action, int mods)
    {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        switch (action) {
        case GLFW_PRESS: {
            KeyPressedEvent event(key, 0);
            data.EventCallback(event);
            break;
        }

        case GLFW_RELEASE: {
            KeyReleasedEvent event(key);
            data.EventCallback(event);
            break;
        }

        case GLFW_REPEAT: {
            KeyPressedEvent event(key, 1);
            data.EventCallback(event);
            break;
        }

        default:
            assert(false); // "Undefined Key action!"
            break;
        }
    });
    
    glfwSetMouseButtonCallback(m_WindowHandle, [](GLFWwindow* window,
                                int button, int action,
                                int mods)
    {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        switch (action) {
        case GLFW_PRESS: {
            MouseButtonPressedEvent event(button);
            data.EventCallback(event);
            break;
        }

        case GLFW_RELEASE: {
            MouseButtonReleasedEvent event(button);
            data.EventCallback(event);
            break;
        }

        default:
            assert(false); // "Undefined Mouse Key action!"
            break;
        }
    });
    
    glfwSetScrollCallback(m_WindowHandle, [](GLFWwindow* window,
                        double xOffset,
                        double yOffset)
    {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        MouseScrolledEvent event((float)xOffset, (float)yOffset);
        data.EventCallback(event);
    });
    
    glfwSetCursorPosCallback(m_WindowHandle, [](GLFWwindow* window,
                            double xPos,
                            double yPos)
    {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        MouseMovedEvent event((float)xPos, (float)yPos);
        data.EventCallback(event);
    });
}

}
