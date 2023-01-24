#include "Window.h"


static bool s_GLFWInitialized = false;

static void error_callback(int error, const char* description)
{
    std::cout << "Error: " << description << std::endl;
}

Window::Window(const WindowProps& props) 
    : _width(props.Width), _height(props.Height)
{
    Init(props);
}

Window::~Window() 
{
    Shutdown();
}

void Window::Init(const WindowProps& props)
{
    if(!s_GLFWInitialized)
    {
        glfwSetErrorCallback(error_callback);
        bool success = glfwInit();
        assert(success); // TODO: add logging 
        s_GLFWInitialized = true;
    }

    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);    
    glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);
 
    _window = glfwCreateWindow(props.Width, props.Height, props.Title.c_str(), NULL, NULL);
    assert(_window != nullptr);    

    glfwMakeContextCurrent(_window);
    glfwSwapInterval(1); 

    glfwSetWindowUserPointer(_window, this);

    auto resizeCB = [](GLFWwindow* w, int width, int height)
    {
        ((Window*)glfwGetWindowUserPointer(w))->OnResize(width, height);
    };

    auto keyCB = [](GLFWwindow* w, int k, int s, int a, int m)
    {
        ((Window*)glfwGetWindowUserPointer(w))->OnKeyInput(k, s, a, m);
    };

    auto mouseCB = [](GLFWwindow* w, int b, int a, int m)
    {
        ((Window*)glfwGetWindowUserPointer(w))->OnMouseInput(b, a, m);
    };

    glfwSetFramebufferSizeCallback(_window, resizeCB);
    glfwSetKeyCallback(_window, keyCB);
    glfwSetMouseButtonCallback(_window, mouseCB);
}

void Window::Shutdown()
{
    glfwDestroyWindow(_window);
    // TODO: glfw should not be terminated if there are other windows
    glfwTerminate();
}

bool Window::ShouldClose() const
{
    return glfwWindowShouldClose(_window);
}
void Window::SwapBuffers()
{
    _resized = false;
    glfwSwapBuffers(_window);
}

void Window::Poll() const
{
    glfwPollEvents();
}

void Window::OnResize(int width, int height)
{
    _width = width;
    _height = height;
    _resized = true;
}

void Window::OnKeyInput(int key, int scancode, int action, int mods)
{
    // TODO: input handling
}

void Window::OnMouseInput(int button, int action, int mods)
{
    // TODO: input handling
}
