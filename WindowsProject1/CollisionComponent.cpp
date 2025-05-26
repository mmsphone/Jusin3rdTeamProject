#include "pch.h"
#include "CollisionComponent.h"
#include "TransformComponent.h"
#include "Object.h"


void CollisionComponent::Initialize()
{
    auto transform = owner ? owner->GetComponent<TransformComponent>() : nullptr;
    if (transform) {
        size.x *= transform->GetScale().x;
        size.y *= transform->GetScale().y;
        offset.x *= transform->GetScale().x;
        offset.y *= transform->GetScale().y;
    }
}

D3DXVECTOR3 CollisionComponent::GetWorldPosition() const {
    auto transform = owner->GetComponent<TransformComponent>();
    if (!transform) return { 0.0f, 0.0f, 0.f };

    return transform->GetPosition() + offset;
}

bool CollisionComponent::CheckCollision(const CollisionComponent* other) const {
    if (this->owner == other->owner) return false;
    auto transform = owner->GetComponent<TransformComponent>();
    auto otherTransform = other->owner->GetComponent<TransformComponent>();
    if (!transform || !otherTransform) return false;

    D3DXVECTOR3 aCenter = this->GetWorldPosition();
    D3DXVECTOR3 bCenter = other->GetWorldPosition();

    D3DXVECTOR3 aHalf = this->size * 0.5f;
    D3DXVECTOR3 bHalf = other->size * 0.5f;

    float dx = std::abs(aCenter.x - bCenter.x);
    float dy = std::abs(aCenter.y - bCenter.y);

    return dx < (aHalf.x + bHalf.x) && dy < (aHalf.y + bHalf.y);
}