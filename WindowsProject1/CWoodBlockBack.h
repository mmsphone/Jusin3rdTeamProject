#pragma once
#include "Object.h"
class CWoodBlockBack :
    public Object
{
public:
    double speed = 10.;
    bool b_CllisionCheak = false;
    bool bBlockCheak = false;
public:
    CWoodBlockBack(ObjectManager*, ObjectType objType, double);
    virtual void Update(double dt);
    virtual void Render(HDC hdc);

private:
    void KeyInput(double dt);
};

