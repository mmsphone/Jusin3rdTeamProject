// InputSystem.cpp
#include "pch.h"
#include "InputSystem.h"

InputSystem::InputSystem() : keyDown(KEY_COUNT, false), wasKeyDown(KEY_COUNT, false) {}

void InputSystem::OnKeyDown(WPARAM key) {
    if (key < KEY_COUNT) {
        keyDown[key] = true;
    }
}

void InputSystem::OnKeyUp(WPARAM key) {
    if (key < KEY_COUNT) {
        keyDown[key] = false;
    }
}

void InputSystem::Tick() {
    wasKeyDown = keyDown; // snapshot of previous frame
}

bool InputSystem::IsKeyDown(WPARAM key) const {
    return isKeyWork && keyDown[key];
}

bool InputSystem::IsKeyPressed(WPARAM key) const {
    return isKeyWork && keyDown[key] && !wasKeyDown[key];
}

bool InputSystem::IsKeyReleased(WPARAM key) const {
    return isKeyWork && !keyDown[key] && wasKeyDown[key];
}
