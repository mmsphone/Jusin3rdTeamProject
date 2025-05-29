
#pragma once
#include "Object.h"
#include "Engine.h"
#include "InputSystem.h"
#include "TransformComponent.h"

class CWoodEndButton : public Object {
public:
    bool bGotoMenu = false;
    double speed = 10.;
    bool bHover = false;
public:
    CWoodEndButton(ObjectManager*, ObjectType objType, double);
    virtual void Update(double dt);
    virtual void Render(HDC hdc);

private:
};