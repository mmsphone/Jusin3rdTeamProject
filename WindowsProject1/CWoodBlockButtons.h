
#pragma once
#include "Object.h"
#include "Random.h"
#include "CWoodBlockShape.h"

class CWoodBlockButtons :
    public Object
{
public:
    CWoodBlockButtons(ObjectManager*, ObjectType objType, double);

    virtual void Update(double dt);
    virtual void Render(HDC hdc);

public:
    double speed = 10.;
    int iBlockType = -1;
    bool b_blockIn = false;
    bool wasRightMouseDown = false;

    float angle = 0;
    

    std::vector<int> blockTypes = { 0,1,2,3,4,5,6,7,8 };
    std::vector<std::shared_ptr<Object>> buttons;
    void BlockTypeShuffle(std::vector<int>& vec);

    std::shared_ptr<CWoodBlockShape> blockshape;
};

