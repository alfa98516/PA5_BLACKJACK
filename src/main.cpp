#include <GL/gl.h>
#include <GL/glew.h>
#include <iostream>

int main() {
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "GLEW init failed: " << glewGetErrorString(err) << std::endl;
        return 1;
    }
    std::cout << "GLEW version: " << glewGetString(GLEW_VERSION) << std::endl;
    return 0;
}
