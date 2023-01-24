#include "ShaderLoader.h"
#include "ImageLoader.h"
#include "Renderer/Renderer.h"
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

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <GL/glew.h>
#include "pch.h"

int main(void)
{
    std::unique_ptr<Window> window = std::make_unique<Window>(WindowProps{
        .Title = "Bloo3D"
    });

    glewInit();

    float pos[] = {
        -1.0f,  1.0f, 0.0f, 0.0f, // ul
         1.0f,  1.0f, 1.0f, 0.0f, // ur
         1.0f, -1.0f, 1.0f, 1.0f, // lr
        -1.0f, -1.0f, 0.0f, 1.0f, // ll
    };

    unsigned int inds[] = {
        0, 1, 2,
        0, 2, 3
    };

    // create vao
    std::unique_ptr<GLVertexArray> va = std::make_unique<GLVertexArray>();
    va->Bind();

    // create vbo
    std::unique_ptr<VertexBuffer> vb;
    vb.reset(VertexBuffer::Create(pos, 16 * sizeof(float))); // for some reason makeunique does not work?!
    vb->Bind();
    // create ibo
    std::unique_ptr<IndexBuffer> ib;
    ib.reset(IndexBuffer::Create(inds, 6));
    ib->Bind();

    VertexBufferLayout layout;
    layout.Push<float>(2);
    layout.Push<float>(2);

    va->AddBuffer(vb.get(), layout);

    std::unique_ptr<Shader> shader;
    shader.reset(Shader::Create("res/Default.shader"));
    shader->Bind();

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
    Renderer renderer(camera.get(), to.get());

    std::unique_ptr<Mesh> mesh = std::make_unique<Mesh>();
    Importer::Import("res/output.obj", mesh.get());
    mesh->BuildBVH();
    renderer.AddMesh(mesh.get());

    std::unique_ptr<MaterialPBR> difMat = std::make_unique<MaterialPBR>();
    difMat->Ambient = glm::vec3(0.1f, 0.1f, 0.1f);
    difMat->Albedo = glm::vec3(0.0f, 0.25f, 0.75f);
    difMat->Metallic = 0;
    difMat->Roughness = .8f;

    int matIndex = renderer.AddMaterial(difMat.get());
    mesh->MaterialIndex = matIndex;


    std::unique_ptr<PointLight> light = std::make_unique<PointLight>(glm::vec3(5, 5, -5), glm::vec3(1, 1, 1), 1);

    renderer.AddLight(light.get());

    // inspector
    std::unique_ptr<Object> obj = std::make_unique<Object>("object");
    std::unique_ptr<ObjectInspector> insp = std::make_unique<ObjectInspector>();

    insp->Bind(obj.get());

    mesh->SetParent(obj.get());

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

    //int frame = 0;
    while (!window->ShouldClose())
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glViewport(0, 0, window->GetWidth(), window->GetHeight());

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if(window->IsResized())
        {
            std::cout << window->GetWidth() << std::endl;
            std::cout << window->GetHeight() << std::endl;

            to->Resize(window->GetWidth(), window->GetHeight());            
            camera->OnResize(window->GetWidth(), window->GetHeight());
            renderer.OnResize(window->GetWidth(), window->GetHeight());
            renderer.Render();
        }
        // draw here
        glDrawElements(GL_TRIANGLES, ib->GetCount(), GL_UNSIGNED_INT, nullptr);
 
        //ImGui::ShowDemoWindow();
        if(ImGui::DragFloat3("pos", &camPos[0]))
        {
            camera->SetPosition(camPos);
        }
        if(ImGui::DragFloat3("rot", &camRot[0]))
        {
            camera->SetRotation(camRot);
        }

        if(ImGui::SliderFloat("metallic", &difMat->Metallic, 0, 1))
        {
//            renderer.Refresh();
        }
        if(ImGui::SliderFloat("roughness", &difMat->Roughness, 0, 1))
        {
//            renderer.Refresh();
        }
        float lp = light->GetPower();
        if(ImGui::SliderFloat("light power", &lp, 1, 100))
        {
            light->SetPower(lp);
            renderer.Render();
        }
        if(ImGui::ColorPicker3("color", &difMat->Albedo[0]))
        {
//            renderer.Render();
        }


        if(insp->OnGUI())
        {
//            renderer.Refresh();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        window->SwapBuffers();
        window->Poll();
    }
 
    // cleanup
    window->Shutdown();

    exit(EXIT_SUCCESS);
}
