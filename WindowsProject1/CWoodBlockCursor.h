
#pragma once
#include "Object.h"
#include "Engine.h"
#include "InputSystem.h"
#include "TransformComponent.h"

class CWoodBlockCursor : public Object {
    double speed = 10.;
    bool b_CllisionCheak = false;
    bool bBlockCheak = false;
public:
    CWoodBlockCursor(ObjectManager*, ObjectType objType, double);
    virtual void Update(double dt);
    virtual void Render(HDC hdc);
};