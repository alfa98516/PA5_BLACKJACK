cmake_minimum_required(VERSION 3.16)
project(FORC_OpenGL_Blackjack VERSION 1.0)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR})
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SOURCES
    src/main.cpp
    src/renderer/Renderer.cpp
    src/renderer/VertexBuffer.cpp
    src/renderer/IndexBuffer.cpp
    src/renderer/VertexArray.cpp
    src/renderer/Shader.cpp
    src/renderer/Texture.cpp
    src/core/Input.cpp
    src/core/ResourceManager.cpp
    src/game/Deck.cpp
    src/game/Hand.cpp
    src/game/Game.cpp
    src/vendor/stb_image/stb_image.cpp
    src/vendor/imgui/imgui.cpp
    src/vendor/imgui/imgui_draw.cpp
    src/vendor/imgui/imgui_impl_glfw.cpp
    src/vendor/imgui/imgui_impl_opengl3.cpp
    src/vendor/imgui/imgui_tables.cpp
    src/vendor/imgui/imgui_widgets.cpp
)



include(cmake/CPM.cmake)

CPMAddPackage("gh:glfw/glfw#3.4")



# GLAD is just a local library now
add_library(glad STATIC glad/src/gl.c)
target_include_directories(glad PUBLIC
    glad/include
)







add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
    src/
    include/
    src/vendor/
)

find_package(OpenGL REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE
  glfw
  glad
  OpenGL::GL
)
