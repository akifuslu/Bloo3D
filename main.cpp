#include "ShaderLoader.h"
#include "ImageLoader.h"
#include "Renderer.h"
#include "Importer.h"
#include "Camera.h"
#include "PointLight.h"
#include "Material.h"
#include "Object.h"
#include "ObjectInspector.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <chrono>
#include <future>
#include <memory>

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}
 
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}


void Init(GLFWwindow*& window, int width, int height)
{
    glfwSetErrorCallback(error_callback);
 
    if (!glfwInit())
        exit(EXIT_FAILURE);
 
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);    
    glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);
 
    window = glfwCreateWindow(width, height, "Simple example", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
 
    glfwSetKeyCallback(window, key_callback);
 
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
 
    glewInit();
}

int main(void)
{
    int width = 800;
    int height = 600;
    GLFWwindow* window;

    Init(window, width, height);

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
    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // create vbo
    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(float), pos, GL_STATIC_DRAW);

    // create ibo
    unsigned int ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), inds, GL_STATIC_DRAW);

    // positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);
    // uvs
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)(sizeof(float) * 2));

    // create shader
    unsigned int shader = ShaderLoader::Load("res/Default.shader");
    glUseProgram(shader);

    // load texture
    //int twidth, theight, channels;
    //unsigned char* tex = ImageLoader::Load("res/Placeholder.jpg", twidth, theight, channels);

    unsigned char* tex = (unsigned char*)malloc(width * height * 4 * sizeof(unsigned int));
    // create to
    unsigned int to;
    glGenTextures(1, &to);
    glBindTexture(GL_TEXTURE_2D, to);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    glm::vec3 camPos(0, 0, -5);
    glm::vec3 camRot(0, 0, 0);
    std::unique_ptr<Camera> camera = std::make_unique<Camera>(
        camPos,
        camRot,
        60,
        glm::ivec2(width, height)
    );
    // create renderer
    Renderer renderer(camera.get(), tex, width, height);

    std::unique_ptr<Mesh> mesh = std::make_unique<Mesh>();
    Importer::Import("res/monkey.obj", mesh.get());
    mesh->BuildBVH();
    renderer.AddMesh(mesh.get());

    // std::unique_ptr<MaterialDiffuse> difMat = std::make_unique<MaterialDiffuse>();
    // difMat->Ambient = glm::vec3(0.1f, 0.1f, 0.1f);
    // difMat->Albedo = glm::vec3(0.0f, 0.25f, 0.75f);

    // int matIndex = renderer.AddMaterial(difMat.get());
    // mesh->MaterialIndex = matIndex;

    std::unique_ptr<MaterialPBR> difMat = std::make_unique<MaterialPBR>();
    difMat->Ambient = glm::vec3(0.1f, 0.1f, 0.1f);
    difMat->Albedo = glm::vec3(0.0f, 0.25f, 0.75f);
    difMat->Metallic = 0;
    difMat->Roughness = .8f;

    int matIndex = renderer.AddMaterial(difMat.get());
    mesh->MaterialIndex = matIndex;


    std::unique_ptr<PointLight> light = std::make_unique<PointLight>(glm::vec3(5, 5, -5), glm::vec3(1, 1, 1), 1);

    renderer.AddLight(light.get());

    renderer.Render();

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
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    int frame = 0;
    glViewport(0, 0, width, height);
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        auto time = std::chrono::high_resolution_clock().now();

        // render
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, tex);

        // draw here
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
 
        frame += 1;
        auto deltaTime = std::chrono::duration<float>(std::chrono::high_resolution_clock().now() - time);

        //std::cout << deltaTime.count() << std::endl;
        // {
        //     ImGui::Begin("Another Window");   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        //     ImGui::Text(std::to_string(deltaTime.count()).c_str());
        //     ImGui::End();
        // }

        ImGui::ShowDemoWindow();

        if(ImGui::Button("Force Refresh"))
        {
            renderer.Refresh();
        }
        if(ImGui::DragFloat3("pos", &camPos[0]))
        {
            camera->SetPosition(camPos);
            renderer.Refresh();
        }
        if(ImGui::DragFloat3("rot", &camRot[0]))
        {
            camera->SetRotation(camRot);
            renderer.Refresh();
        }

        if(ImGui::SliderFloat("metallic", &difMat->Metallic, 0, 1))
        {
            renderer.Refresh();
        }
        if(ImGui::SliderFloat("roughness", &difMat->Roughness, 0, 1))
        {
            renderer.Refresh();
        }
        float lp = light->GetPower();
        if(ImGui::SliderFloat("light power", &lp, 1, 100))
        {
            light->SetPower(lp);
            renderer.Refresh();
        }
        if(ImGui::ColorPicker3("color", &difMat->Albedo[0]))
        {
            renderer.Refresh();
        }


        if(insp->OnGUI())
        {
            renderer.Refresh();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
 
    // cleanup
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ibo);
    glDeleteTextures(1, &to);
    free(tex);

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
