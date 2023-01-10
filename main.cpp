#include "ShaderLoader.h"
#include "ImageLoader.h"
#include "Renderer.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <chrono>

 
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
    int width = 1024;
    int height = 768;
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
    int twidth, theight, channels;
    unsigned char* tex = ImageLoader::Load("res/Placeholder.jpg", twidth, theight, channels);

    // create to
    unsigned int to;
    glGenTextures(1, &to);
    glBindTexture(GL_TEXTURE_2D, to);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, twidth, theight, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex);

    // create renderer
    Renderer renderer;

    int frame = 0;
    glViewport(0, 0, width, height);
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        auto time = std::chrono::high_resolution_clock().now();

        // render
        renderer.Render(tex, twidth, theight, frame);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, twidth, theight, GL_RGBA, GL_UNSIGNED_BYTE, tex);

        // draw here
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
 
        frame += 1;
        auto deltaTime = std::chrono::duration<float, std::milli>(std::chrono::high_resolution_clock().now() - time);

        //std::cout << deltaTime.count() << std::endl;

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
 
    // cleanup
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ibo);


    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
