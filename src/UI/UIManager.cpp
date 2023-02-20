#include "UIManager.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

UIManager::UIManager(Window* window) 
{
    Init(window);    
}

void UIManager::NewFrame() 
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();    
    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);
}

void UIManager::Render() 
{
    _hierView.OnGUI();
    _inspView.OnGUI();
    static bool sdf;
    ImGui::ShowDemoWindow(&sdf);
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());    
}

void UIManager::SetScene(Scene* scene)
{
    _scene = scene;
    _hierView.Bind(_scene);
    _scene->activeObject.Subscribe([&](Object* sel)
    {
        if(sel == nullptr)
        {
            _inspView.UnBind();
        }
        else
        {
            _inspView.Bind(sel);
        }
    });
}

void UIManager::Init(Window* window) 
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;    
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    const char* glsl_version = "#version 410";
    ImGui_ImplGlfw_InitForOpenGL(window->Get(), true);
    ImGui_ImplOpenGL3_Init(glsl_version);    
}

