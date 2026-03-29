#include "Input.hpp"
#include <GLFW/glfw3.h>
#include <ios>
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

void Input::ConsumeClick() { leftButtonWasPressed = false; }

void Input::SetWindow(GLFWwindow* w) { window = w; }

bool Input::PeekClicked() const { return leftButtonWasPressed; }

void Input::RegisterCallbacks() { glfwSetMouseButtonCallback(window, MouseButtonCallback); }
