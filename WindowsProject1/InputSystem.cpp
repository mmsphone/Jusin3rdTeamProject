﻿// InputSystem.cpp
#include "pch.h"
#include "InputSystem.h"
#include "Engine.h"

InputSystem::InputSystem() : keyDown(KEY_COUNT, false), wasKeyDown(KEY_COUNT, false), mouseDown(false), wasMouseDown(false)
{}

void InputSystem::OnMouseDown()
{
    mouseDown = true;
}

void InputSystem::OnMouseUp()
{
    mouseDown = false;
}
void InputSystem::Tick() {
    for (int i = 0; i < KEY_COUNT; ++i)
    {
        wasKeyDown[i] = keyDown[i];
        keyDown[i] = (GetAsyncKeyState(i) & 0x8000) != 0;
    }    
    wasMouseDown = mouseDown;
}

bool InputSystem::IsKeyDown(WPARAM key) const {
    return isKeyWork && key < KEY_COUNT&& keyDown[key];
}

bool InputSystem::IsKeyPressed(WPARAM key) const {
    return isKeyWork && key < KEY_COUNT && keyDown[key] && !wasKeyDown[key];
}

bool InputSystem::IsKeyReleased(WPARAM key) const {
    return isKeyWork && key < KEY_COUNT && !keyDown[key] && wasKeyDown[key];
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

D3DXVECTOR3 InputSystem::GetMousePos()
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
