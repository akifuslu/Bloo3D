cmake_minimum_required(VERSION 3.0.0)
project(BlooRay VERSION 0.1.0)

set (CMAKE_CXX_STANDARD 17)

add_executable(BlooRay main.cpp Renderer.cpp external/stb_init.cpp)

include_directories(external)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)

target_link_libraries(BlooRay glfw OpenGL::GL GLEW::GLEW)

add_custom_target(copy_resources ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/res
        ${PROJECT_BINARY_DIR}/res
        COMMENT "Copying resources into binary directory")

add_dependencies(BlooRay copy_resources)


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
