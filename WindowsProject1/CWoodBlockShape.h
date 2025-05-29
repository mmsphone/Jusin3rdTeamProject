
#pragma once
#include "Object.h"

class CWoodBlockShape :
    public Object
{
public:
    typedef struct BlockInfo
    {
        std::shared_ptr<Object> block;
        D3DXVECTOR3 relativeOffset;
    }Block;

public:
    CWoodBlockShape(ObjectManager*, ObjectType objType, double speed, int _iBlockType);

    virtual void Update(double dt);
    virtual void Render(HDC hdc);

public:
    double speed = 10.;
    int iGridW = 3;
    int iGridH = 3;
    float fWoodBlockSize = 40.f;
    int iBlockType = 0;

    std::vector<BlockInfo> blocks;

    int iScore = 0;
    double dt;

    void RequestRotate();
private:
    float currentAngle = 0.0f;
    float targetAngle = 0.0f;
    bool isRotating = false;

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

