#pragma once
#include "ObjectComponent.h"
class CollisionComponent : public ObjectComponent {
public:
    math::vec2 offset = { 0.0f, 0.0f };      // Transform에서 얼마나 떨어진 위치
    math::vec2 size = { 1.f, 1.f };      // 충돌 박스 크기

    bool isTrigger = false;           // 충돌 후 반응 여부 결정 (물리 반응 유무)

public:
    CollisionComponent() = default;
    virtual ~CollisionComponent() override = default;
    virtual void Initialize();
    virtual void Update(double deltaTime) override {};

    math::vec2 GetWorldPosition() const;

    bool CheckCollision(const CollisionComponent* other) const;
};