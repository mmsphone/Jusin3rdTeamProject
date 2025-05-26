#include "pch.h"
#include "CollisionComponent.h"
#include "TransformComponent.h"
#include "Object.h"


void CollisionComponent::Initialize() {
    auto transform = owner ? owner->GetComponent<TransformComponent>() : nullptr;
    if (transform) {
        static const D3DXVECTOR3 baseSize = size;
        static const D3DXVECTOR3 baseOffset = offset;

        size.x = baseSize.x * transform->GetScale().x;
        size.y = baseSize.y * transform->GetScale().y;
        offset.x = baseOffset.x * transform->GetScale().x;
        offset.y = baseOffset.y * transform->GetScale().y;
    }
}

D3DXVECTOR3 CollisionComponent::GetWorldPosition() const {
    auto transform = owner->GetComponent<TransformComponent>();
    if (!transform) return { 0.0f, 0.0f, 0.f };

    return transform->GetPosition() + offset;
}

bool CollisionComponent::CheckCollision(const CollisionComponent* other) const {
    if (!other || other == this) return false;

    auto transform = owner ? owner->GetComponent<TransformComponent>() : nullptr;
    auto otherTransform = other->owner ? other->owner->GetComponent<TransformComponent>() : nullptr;
    if (!transform || !otherTransform) return false;

    D3DXVECTOR3 aCenter = GetWorldPosition();
    D3DXVECTOR3 bCenter = other->GetWorldPosition();

    D3DXVECTOR3 aHalf = size * 0.5f;
    D3DXVECTOR3 bHalf = other->size * 0.5f;

    float dx = std::abs(aCenter.x - bCenter.x);
    float dy = std::abs(aCenter.y - bCenter.y);

    return dx < (aHalf.x + bHalf.x) && dy < (aHalf.y + bHalf.y);
}