#include "pch.h"
#include "CollisionComponent.h"
#include "TransformComponent.h"
#include "Object.h"


void CollisionComponent::Initialize()
{
    auto transform = owner ? owner->GetComponent<TransformComponent>() : nullptr;
    if (transform) {
        size.x *= transform->scale.x;
        size.y *= transform->scale.y;
    }
}

math::vec2 CollisionComponent::GetWorldPosition() const {
    auto transform = owner->GetComponent<TransformComponent>();
    if (!transform) return { 0.0f, 0.0f };

    return transform->position + offset;
}

bool CollisionComponent::CheckCollision(const CollisionComponent* other) const {
    auto transform = owner->GetComponent<TransformComponent>();
    auto otherTransform = other->owner->GetComponent<TransformComponent>();

    math::vec2 aCenter = transform->GetPosition();
    math::vec2 bCenter = otherTransform->GetPosition();

    math::vec2 aHalf = this->size * 0.5f;
    math::vec2 bHalf = other->size * 0.5f;

    float dx = std::abs(aCenter.x - bCenter.x);
    float dy = std::abs(aCenter.y - bCenter.y);

    return dx < (aHalf.x + bHalf.x) && dy < (aHalf.y + bHalf.y);
}