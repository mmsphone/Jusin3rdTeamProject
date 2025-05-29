
#pragma once
#include "Object.h"
#include "Engine.h"
#include "InputSystem.h"
#include "TransformComponent.h"

class CWoodBlockButton : public Object {
public:
    double speed = 10.;
    bool bHover = false;
    int iBlockType;
    std::shared_ptr<Object> blockshape;
    bool isSelected = false;
    bool isBlockin = false;

    bool WasJustClicked() const { return wasJustClicked; }
    void ClearClickFlag() { wasJustClicked = false; }

public:
    CWoodBlockButton(ObjectManager*, ObjectType objType, double, int _iBlockType);
    virtual void Update(double dt);
    virtual void Render(HDC hdc);

private:
    bool prevMouseDown = false;
    bool wasJustClicked = false;
    // void KeyInput(double dt);
};