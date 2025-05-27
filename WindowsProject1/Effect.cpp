#include "pch.h"
#include "Effect.h"

Effect::Effect(ObjectManager* owner, ObjectType type)
    : Object(owner, type), lifetime(0.0), elapsedTime(0.0) {
}

void Effect::Update(double dt) {
    elapsedTime += dt;
    if (elapsedTime >= lifetime) {
        SetDead();  // 시간이 다 되면 이펙트를 죽임
    }
}