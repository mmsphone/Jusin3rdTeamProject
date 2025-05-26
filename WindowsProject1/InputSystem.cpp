// InputSystem.cpp
#include "pch.h"
#include "InputSystem.h"
#include "Engine.h"

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
void InputSystem::OnMouseMove(int x, int y)
{
    mousePos = { x, y };
}

void InputSystem::OnMouseDown()
{
    mouseDown = true;
}

void InputSystem::OnMouseUp()
{
    mouseDown = false;
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
bool InputSystem::IsMouseDown() const {
    return isMouseWork && mouseDown;
}

bool InputSystem::IsMousePressed() const {
    return isMouseWork && mouseDown && !wasMouseDown;
}

bool InputSystem::IsMouseReleased() const {
    return isMouseWork && !mouseDown && wasMouseDown;
}

math::vec2 InputSystem::GetMousePos()
{
    POINT pt;
    GetCursorPos(&pt);
    ScreenToClient(Engine::GetInstance().GethWnd(), &pt);
    mousePos.x = pt.x;
    mousePos.y = pt.y;
    return mousePos;
}

void InputSystem::SetKeyWork(bool keyWork)
{
    isKeyWork = keyWork;
}

void InputSystem::SetMouseWork(bool mouseWork)
{
    isMouseWork = mouseWork;
}
