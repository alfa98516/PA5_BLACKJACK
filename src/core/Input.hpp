#pragma once
#include "glm/ext/vector_double2.hpp"
#include <GLFW/glfw3.h>
#include <iostream>

class Input {
    GLFWwindow* window = nullptr;
    Input() = default;
    glm::dvec2 position;
    bool leftButtonWasPressed = false;

    static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {

        Instance().onMouseButton(button, action, mods);
    }
    void onMouseButton(int button, int action, int mods);

  public:
    void SetWindow(GLFWwindow*);
    void RegisterCallbacks();
    Input(const Input&) = delete;
    Input& operator=(const Input&) = delete;
    static Input& Instance() {
        static Input instance;
        return instance;
    }

    void Update();
    const glm::dvec2& GetPosition() const;
    bool WasClicked();
    bool PeekClicked() const;
    void ConsumeClick();
};
