cmake_minimum_required(VERSION 3.0.0)
project(Bloo3D VERSION 0.1.0)

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_FLAGS "-O3 -Wall -fsanitize=address")


set (SRC 
        src/main.cpp
        src/Window.cpp
        src/Input.cpp
        src/Renderer/Raytracer.cpp
        src/Renderer/GLRenderer.cpp
        src/Camera/Camera.cpp
        src/Geometry/Mesh.cpp
        src/Geometry/ScreenQuad.cpp
        src/Light/Light.cpp
        src/Material/Material.cpp
        src/Scene/Object.cpp
        src/Scene/Scene.cpp
        src/UI/UIManager.cpp
        src/UI/HierarchyView.cpp
        src/UI/InspectorView.cpp
        src/UI/TransformInspector.cpp
        src/UI/ObjectInspector.cpp
        src/UI/LightInspector.cpp
        src/GPU/Buffer.cpp        
        src/GPU/VertexBufferLayout.cpp
        src/GPU/Shader.cpp
        src/GPU/Texture.cpp
        src/GPU/GL/GLVertexBuffer.cpp
        src/GPU/GL/GLIndexBuffer.cpp
        src/GPU/GL/GLVertexArray.cpp
        src/GPU/GL/GLShader.cpp
        src/GPU/GL/GLTexture.cpp
        src/GPU/GL/GLUniformBuffer.cpp
        src/Interactors/ObjectModeInteractor.cpp
        src/ResourceManager.cpp
        external/imgui/imgui_demo.cpp
        external/imgui/imgui_draw.cpp
        external/imgui/imgui_impl_glfw.cpp
        external/imgui/imgui_impl_opengl3.cpp
        external/imgui/imgui_tables.cpp
        external/imgui/imgui_widgets.cpp
        external/imgui/imgui.cpp
        external/stb/stb_init.cpp    
)

set (INC
        src
        external
        external/stb
        external/imgui
)

add_executable(Bloo3D ${SRC})

include_directories(${INC})

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(assimp REQUIRED)

target_link_libraries(Bloo3D glfw OpenGL::GL GLEW::GLEW assimp::assimp)

add_custom_target(copy_resources ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/res
        ${PROJECT_BINARY_DIR}/res
        COMMENT "Copying resources into binary directory")

add_dependencies(Bloo3D copy_resources)


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
