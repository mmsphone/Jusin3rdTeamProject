

#pragma once
#include "Object.h"
#include "Engine.h"
#include "InputSystem.h"
#include "TransformComponent.h"

class CWoodEndUI : public Object {
public:
    double speed = 10.;
    int score;
    std::shared_ptr<Object> blockshape;

public:
    CWoodEndUI(ObjectManager*, ObjectType objType, double);
    virtual void Update(double dt);
    virtual void Render(HDC hdc);

};