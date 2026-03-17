cmake_minimum_required(VERSION 3.0.0)
project(opengl_FORC_blackjack VERSION 0.0.1)
cmake_policy(SET CMP0072 NEW)
include(FetchContent)


FetchContent_Declare(
  glew
  URL https://github.com/nigels-com/glew/releases/download/glew-2.2.0/glew-2.2.0.tgz
)

FetchContent_MakeAvailable(glew)
find_package(OpenGL REQUIRED)


add_executable(opengl_FORC_blackjack
    main.cpp
)

target_link_libraries(opengl_FORC_blackjack
    PRIVATE
    glfw
    OpenGL::GL
    GLEW::glew
)

