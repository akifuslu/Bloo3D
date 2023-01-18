cmake_minimum_required(VERSION 3.0.0)
project(BlooRay VERSION 0.1.0)

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_FLAGS "-O0")


set (SRC 
        main.cpp
        Renderer.cpp
        Camera.cpp
        Mesh.cpp
        PointLight.cpp
        Material.cpp
        Object.cpp
        ObjectInspector.cpp
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
        external
        external/stb
        external/imgui
)

add_executable(BlooRay ${SRC})

include_directories(${INC})

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(assimp REQUIRED)

target_link_libraries(BlooRay glfw OpenGL::GL GLEW::GLEW assimp::assimp)

add_custom_target(copy_resources ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/res
        ${PROJECT_BINARY_DIR}/res
        COMMENT "Copying resources into binary directory")

add_dependencies(BlooRay copy_resources)


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
