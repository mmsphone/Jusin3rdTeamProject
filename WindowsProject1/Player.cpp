#include "pch.h"
#include "Player.h"
#include "TransformComponent.h"
#include "CollisionComponent.h"
#include "Engine.h"
#include "InputSystem.h"
#include "ObjectManager.h"
#include "Bullet.h"

Player::Player(ObjectManager* owner, ObjectType objType, double speed)
    : Object(owner, objType, RenderType::Rect), speed(speed)
{
    auto transform = AddComponent<TransformComponent>();
    transform->SetPosition(400.0f, 300.0f);
    transform->SetScale(100.0f, 100.0f);

    auto collision = AddComponent<CollisionComponent>();
    collision->Initialize();

}

void Player::Update(double dt)
{
    auto transform = GetComponent<TransformComponent>();
    if (!transform) return;

    auto input = Engine::GetInstance().GetInputSystem(); 

    if (input->IsKeyDown(VK_UP)) {
        transform->Translate(0, -speed * static_cast<float>(dt));
    }
    if (input->IsKeyDown(VK_DOWN)) {
        transform->Translate(0, speed * static_cast<float>(dt));
    }
    if (input->IsKeyDown(VK_LEFT)) {
        transform->Translate(-speed * static_cast<float>(dt), 0);
    }
    if (input->IsKeyDown(VK_RIGHT)) {
        transform->Translate(speed * static_cast<float>(dt), 0);
    }

    if (input->IsKeyPressed(VK_SPACE))
    {
    }
}
