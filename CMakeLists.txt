cmake_minimum_required(VERSION 3.16)
project(FORC_OpenGL_Blackjack VERSION 1.0)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(cmake/CPM.cmake)

CPMAddPackage("gh:glfw/glfw#3.4")

# GLAD is just a local library now
add_library(glad STATIC glad/src/gl.c)
target_include_directories(glad PUBLIC glad/include)

add_executable(${PROJECT_NAME} src/main.cpp)

find_package(OpenGL REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE
  glfw
  glad
  OpenGL::GL
)
