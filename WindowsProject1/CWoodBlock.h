
#pragma once
#include "Object.h"
class CWoodBlock :
    public Object
{
    double speed = 10.;
public:
    CWoodBlock(ObjectManager*, ObjectType objType, double);
    virtual void Update(double dt);
    virtual void Render(HDC hdc);

private:
    void KeyInput(double dt);
};

