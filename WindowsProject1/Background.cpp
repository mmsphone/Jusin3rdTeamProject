#include "pch.h"
#include "Background.h"
#include "TransformComponent.h"
#include "CollisionComponent.h"
#include "Engine.h"

Background::Background(ObjectManager* owner, ObjectType objType) : Object(owner, objType, RenderType::Rect)
{
    auto transform = AddComponent<TransformComponent>();
    transform->SetPosition(400.0f, 300.0f);
    RECT windowSize = Engine::GetInstance().GetWindowSize();
    transform->SetScale(windowSize.right - windowSize.left - 100, windowSize.bottom - windowSize.top - 100);

    auto collision = AddComponent<CollisionComponent>();
    collision->Initialize();
}

