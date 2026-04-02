cmake_minimum_required(VERSION 3.16)
project(FORC_OpenGL_Blackjack VERSION 1.0)

# Build configuration
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR})
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Disable GLFW examples/tests/docs
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)

# Source files
set(SOURCES
    src/main.cpp
    src/renderer/Renderer.cpp
    src/renderer/VertexBuffer.cpp
    src/renderer/IndexBuffer.cpp
    src/renderer/VertexArray.cpp
    src/renderer/Shader.cpp
    src/renderer/Texture.cpp
    src/renderer/VertexBufferLayout.cpp
    src/renderer/TextRenderer.cpp
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

# Include CPM to fetch GLFW
include(cmake/CPM.cmake)

# Fetch GLFW
CPMAddPackage("gh:glfw/glfw#3.4")

# Build GLAD locally (if glad sources are in glad/ subfolder)
add_library(glad STATIC glad/src/gl.c)
target_include_directories(glad PUBLIC glad/include)

# Create the executable
add_executable(${PROJECT_NAME} ${SOURCES})


# OpenGL
find_package(OpenGL REQUIRED)

set(FREETYPE_DIR "${CMAKE_SOURCE_DIR}/libs/freetype/install")
# Use the static library directly (ensure it exists)
set(FREETYPE_LIB "${FREETYPE_DIR}/lib/libfreetype.a")

# Include FreeType headers
target_include_directories(${PROJECT_NAME} PRIVATE
    ${FREETYPE_DIR}/include/freetype2
    ${FREETYPE_DIR}/include/
)

# Define FT_STATIC_LIBRARY to avoid DLL import/export issues on Windows (harmless on Unix)
target_compile_definitions(${PROJECT_NAME} PRIVATE FT_STATIC_LIBRARY)

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    glfw
    glad
    OpenGL::GL
    ${FREETYPE_LIB}
)


# Project-specific include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    src/
    include/
    src/vendor/
)
