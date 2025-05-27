#pragma once
#include "ObjectComponent.h"
class CollisionComponent : public ObjectComponent {
public:
    D3DXVECTOR3 offset = { 0.0f, 0.0f, 0.0f };      // Transform���� �󸶳� ������ ��ġ
    D3DXVECTOR3 size = { 1.f, 1.f, 0.f };      // �浹 �ڽ� ũ��

    bool isTrigger = false;           // �浹 �� ���� ���� ���� (���� ���� ����)

public:
    CollisionComponent() = default;
    virtual ~CollisionComponent() override = default;
    virtual void Initialize();
    virtual void Update(double deltaTime) override {};

    D3DXVECTOR3 GetWorldPosition() const;

    bool CheckCollision(const CollisionComponent* other) const;
};