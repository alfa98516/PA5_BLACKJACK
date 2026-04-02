#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <iostream>

#include "core/Input.hpp"
#include "core/ResourceManager.hpp"
#include "game/Game.hpp"
#include "renderer/Macros.hpp"
#include "renderer/Renderer.hpp"

int main(int argc, char* argv[]) {

    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    // set minimum version of GLFW and set the profile to core
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw handles windows(not the os), they're freaky and different for every operating system.
    GLFWwindow* window =
        glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Blackjack", nullptr, nullptr);

    if (!window) {
        std::cerr << "Failed to initialize GLFW window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGL(glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        glfwTerminate();
    }

    // Turn on vsync
    // side note: if you're using an NVIDIA graphics card, this might not work.
    // it gets completely overwritten by some NVIDIA drivers
    // So if you happen to notice that the game speed is unusually high, and you're using an NVIDIA
    // card, thats why. Don't buy NVIDIA cards !
    glfwSwapInterval(1);

    // disable byte-alignment restriction
    GLCall(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));

    // debug
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << "\n";

    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    {
        Input& input = Input::Instance();
        input.SetWindow(window);
        input.RegisterCallbacks();
        ResourceManager& resourceManager = ResourceManager::Instance();

        Renderer& renderer = Renderer::Instance();

        Game game = Game(renderer, resourceManager);

        while (!glfwWindowShouldClose(window)) {
            renderer.Clear();
            input.Update();
            game.UpdateState();

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
        renderer.Shutdown();
        resourceManager.Shutdown();
    }

    glfwTerminate();
    return 0;
}
