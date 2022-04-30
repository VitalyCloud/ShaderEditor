//
//  Context.cpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 30.04.2022.
//
#include "pch.h"
#include "Context.hpp"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace OpenGL {

Context::Context(GLFWwindow* windowHandle) : m_WindowHandle(windowHandle) {
    EN_CORE_ASSERT(windowHandle)
}

void Context::Init() {
    glfwMakeContextCurrent(m_WindowHandle);
    
    int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    EN_CORE_ASSERT(status, "Failed to initiaalize Glad!");
    
    EN_CORE_INFO("OpenGL Info:");
    EN_CORE_INFO("\tVendor: {0}", glGetString(GL_VENDOR));
    EN_CORE_INFO("\tRenderer: {0}", glGetString(GL_RENDERER));
    EN_CORE_INFO("\tVersion: {0}", glGetString(GL_VERSION));
    
    EN_CORE_ASSERT(GLVersion.major > 4 || (GLVersion.major == 4 && GLVersion.minor >= 1), "Require at at least OpenGL version 4.1");
}


}
