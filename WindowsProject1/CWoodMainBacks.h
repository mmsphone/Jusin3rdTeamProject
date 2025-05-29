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

    int iScore = 0;
    double dt;

private:

    std::vector<std::vector<bool>> RotateShape(int shape, float radians);
    // std::unique_ptr<ObjectManager> objectManager;
    const std::vector<std::vector<bool>> blockShapes[9] = {
    {
        { true, false, false },
        { true, false, false },
        { true, true,  false }
    },
    {
        { false, false, true },
        { false, false, true },
        { false, true,  true }
    },
    {
        { true, true, false },
        { true, true, false },
        { false, false, false }
    },
    {
        { false, true,  false },
        { true,  true,  true  },
        { false, false, false }
    },
    {
        { true,  false, false },
        { true, false, false },
        { true,  true,  true  }
    },
    {
        { false, true,  true },
        { true,  true,  false },
        { false, false, false }
    },
    {
        { false,  true,  false },
        { true, true,  true  },
        { false, true, false }
    },
    {
        { false,  false,  false },
        { false, true,  false  },
        { false, false, false }
    },
    {
        { false,  false,  false },
        { true, true,  true  },
        { false, false, false }
    }
    };
};

