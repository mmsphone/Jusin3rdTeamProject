#pragma once
#include "Object.h"

class CWoodMainBacks :
    public Object
{
public:

    typedef struct BlockInfo
    {
        bool bBlockCheak;
        bool bCollisionCheak;
        std::shared_ptr<Object> block;
    }Block;

public:
    CWoodMainBacks(ObjectManager*, ObjectType objType, double);

    virtual void Update(double dt);
    virtual void Render(HDC hdc);

public:
    double speed = 10.;
    int iGridW = 10;
    int iGridH = 10;
    float fWoodBlockSize = 40.f;

    std::vector<Block> blocks;

private:
    // std::unique_ptr<ObjectManager> objectManager;



};

