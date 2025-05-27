#pragma once
#include "Object.h"

class Effect : public Object
{
protected:
    double lifetime;  // 이펙트의 지속 시간
    double elapsedTime;  // 경과 시간

public:
    Effect(ObjectManager* owner, ObjectType type);

    virtual ~Effect() {}

    virtual void Update(double dt);

    virtual void Render(HDC hdc) = 0;
};