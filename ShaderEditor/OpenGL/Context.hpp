//
//  Context.hpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 30.04.2022.
//

#ifndef Context_hpp
#define Context_hpp

struct GLFWwindow;

namespace OpenGL {

typedef void (*GLFWglproc)(void);
typedef GLFWglproc(*GLFWproc)(const char*);

class Context {
public:
    Context(GLFWwindow* windowHandle);
    
    void Init();
    void SwapBuffers();
    
private:
    GLFWwindow* m_WindowHandle = nullptr;
};


}

#endif /* Context_hpp */
