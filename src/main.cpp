#include "ShaderLoader.h"
#include "ImageLoader.h"
#include "Renderer/Raytracer.h"
#include "Importer.h"
#include "Camera/Camera.h"
#include "Light/PointLight.h"
#include "Material/Material.h"
#include "Scene/Object.h"
#include "UI/ObjectInspector.h"
#include "Window.h"
#include "GPU/Buffer.h"
#include "GPU/GL/GLVertexArray.h"
#include "GPU/VertexBufferLayout.h"
#include "GPU/Shader.h"
#include "GPU/Texture.h"
#include "Geometry/ScreenQuad.h"
#include "Renderer/GLRenderer.h"
#include "Scene/Scene.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Logger.h"

#include "pch.h"

int main(void)
{
    std::unique_ptr<Window> window = std::make_unique<Window>(WindowProps{
        .Title = "Bloo3D"
    });

    std::unique_ptr<GLRenderer> renderer = std::make_unique<GLRenderer>();

    renderer->SetMode(GLRenderMode::RAYTRACER);
    renderer->OnResize(window->GetWidth(), window->GetHeight());

    std::unique_ptr<Texture> to;
    to.reset(Texture::Create({
        .Unit = 0,
        .Width = window->GetWidth(),
        .Height = window->GetHeight()
    }));

    glm::vec3 camPos(0, 0, -5);
    glm::vec3 camRot(0, 0, 0);
    std::unique_ptr<Camera> camera = std::make_unique<Camera>(
        camPos,
        camRot,
        60,
        window->GetWidth(),
        window->GetHeight()
    );
    // create renderer
    Raytracer raytracer(camera.get(), to.get());

    std::unique_ptr<Mesh> mesh = std::make_unique<Mesh>();
    Importer::Import("res/monkey.obj", mesh.get());
    mesh->BuildBVH();
    mesh->transform.SetRotation(Transform::EulerToQuat(glm::vec3(0, 180, 0)));
    raytracer.AddMesh(mesh.get());

    std::unique_ptr<MaterialPBR> difMat = std::make_unique<MaterialPBR>();
    difMat->Ambient = glm::vec3(0.1f, 0.1f, 0.1f);
    difMat->Albedo = glm::vec3(0.0f, 0.25f, 0.75f);
    difMat->Metallic = 0;
    difMat->Roughness = .8f;

    int matIndex = raytracer.AddMaterial(difMat.get());
    mesh->MaterialIndex = matIndex;


    std::unique_ptr<PointLight> light = std::make_unique<PointLight>(glm::vec3(1, 1, 1), 100);
    light->transform.SetLocation(glm::vec3(0, 5, -5));

    raytracer.AddLight(light.get());

    // imgui setup
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    const char* glsl_version = "#version 410";
    ImGui_ImplGlfw_InitForOpenGL(window->Get(), true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    Scene testScene;
    while (!window->ShouldClose())
    {
        renderer->Clear();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if(window->IsResized())
        {
            std::cout << window->GetWidth() << std::endl;
            std::cout << window->GetHeight() << std::endl;

            to->Resize(window->GetWidth(), window->GetHeight());            
            camera->OnResize(window->GetWidth(), window->GetHeight());
            raytracer.OnResize(window->GetWidth(), window->GetHeight());
            raytracer.Render();
        }
        renderer->Render(testScene);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        window->SwapBuffers();
        window->Poll();
    }
 
    // cleanup
    window->Shutdown();

    exit(EXIT_SUCCESS);
}
