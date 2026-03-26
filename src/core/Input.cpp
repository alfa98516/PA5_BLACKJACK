#include "Input.hpp"
#include <GLFW/glfw3.h>
void Input::onMouseButton(int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        leftButtonWasPressed = true;
}
void Input::Update() { glfwGetCursorPos(window, &position.x, &position.y); }
const glm::dvec2& Input::GetPosition() const { return position; }
bool Input::WasClicked() {
    bool pressed = leftButtonWasPressed;
    leftButtonWasPressed = false;
    return pressed;
}
void Input::SetWindow(GLFWwindow* w) { window = w; }

void Input::RegisterCallbacks() { glfwSetMouseButtonCallback(window, MouseButtonCallback); }
