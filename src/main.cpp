#include "ShaderLoader.h"
#include "ImageLoader.h"
#include "Renderer/Raytracer.h"
#include "Importer.h"
#include "Camera/Camera.h"
#include "Light/PointLight.h"
#include "Material/Material.h"
#include "Scene/Object.h"
#include "UI/TransformInspector.h"
#include "Window.h"
#include "GPU/Texture.h"
#include "Geometry/ScreenQuad.h"
#include "Renderer/GLRenderer.h"
#include "Scene/Scene.h"
#include "UI/UIManager.h"
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
        .unit = 0,
        .width = window->GetWidth(),
        .height = window->GetHeight()
    }));

    std::unique_ptr<Camera> camera = std::make_unique<Camera>(
        60,
        0.1f,
        100.0f,
        window->GetWidth(),
        window->GetHeight()
    );

    camera->transform.SetLocation(glm::vec3(0, 2, -5));
    camera->transform.SetRotation(Transform::EulerToQuat(glm::vec3(15, 0, 0)));

    TransformInspector insp;
    insp.Bind(&camera->transform);

    // create renderer
    Raytracer raytracer(camera.get(), to.get());

    std::unique_ptr<Mesh> mesh = std::make_unique<Mesh>();
    Importer::Import("res/monkey.obj", mesh.get());
    mesh->BuildBVH();
    raytracer.AddMesh(mesh.get());

    std::unique_ptr<MaterialPBR> difMat = std::make_unique<MaterialPBR>();
    difMat->ambient = glm::vec3(0.1f, 0.1f, 0.1f);
    difMat->albedo = glm::vec3(0.0f, 0.25f, 0.75f);
    difMat->metallic = 0;
    difMat->roughness = .8f;

    int matIndex = raytracer.AddMaterial(difMat.get());
    mesh->materialIndex = matIndex;


    std::unique_ptr<PointLight> light = std::make_unique<PointLight>(glm::vec3(1, 1, 1), 100);
    light->transform.SetLocation(glm::vec3(0, 5, -5));

    raytracer.AddLight(light.get());

    std::unique_ptr<UIManager> uiManager = std::make_unique<UIManager>(window.get());

    Scene testScene;
    testScene.mainCam = camera.get();
    testScene.meshes.push_back(mesh.get());
    renderer->SetMode(GLRenderMode::DEFAULT);
    while (!window->ShouldClose())
    {
        renderer->Clear();
        uiManager->NewFrame();

        if(window->IsResized())
        {
            to->Resize(window->GetWidth(), window->GetHeight());            
            camera->OnResize(window->GetWidth(), window->GetHeight());
            raytracer.OnResize(window->GetWidth(), window->GetHeight());
            raytracer.Render();
        }

        renderer->Render(testScene);
        insp.OnGUI();
        uiManager->Render();

        window->SwapBuffers();
        window->Poll();

        int Estate = glfwGetKey(window->Get(), GLFW_KEY_E);
        if (Estate == GLFW_PRESS)
        {
            renderer->SetMode(GLRenderMode::RAYTRACER);
            raytracer.Render();
        }
        int Qstate = glfwGetKey(window->Get(), GLFW_KEY_Q);
        if (Qstate == GLFW_PRESS)
        {
            renderer->SetMode(GLRenderMode::DEFAULT);
        }

    }
 
    // cleanup
    window->Shutdown();

    exit(EXIT_SUCCESS);
}
