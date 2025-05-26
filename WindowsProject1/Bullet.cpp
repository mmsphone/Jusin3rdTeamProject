#include "pch.h"
#include "Bullet.h"
#include "TransformComponent.h"
#include "CollisionComponent.h"
#include "ObjectManager.h"
#include "Engine.h"
#include "SceneManager.h"
#include "Scene.h"
#include "ObjectManager.h"

Bullet::Bullet(ObjectManager* owner, ObjectType objType, double speed) : Object(owner, objType, RenderType::Ellipse), speed(speed)
{
    auto collision = AddComponent<CollisionComponent>();
    collision->Initialize();
}

void Bullet::Render(HDC hdc)
{
    Object::Render(hdc);
}

void Bullet::Update(double dt)
{
    //위치 업데이트
    auto transform = GetComponent<TransformComponent>();
    float currRotate = transform->GetRotation();
    math::RotateMatrix rotMat(currRotate);
    
    math::vec2 velocity = rotMat * math::vec2(speed, speed) * dt;
    transform->Translate(velocity.x, velocity.y);

    //충돌 업데이트
    auto collision = GetComponent<CollisionComponent>();
    
    bool backgroundCollisionCheck = collision->CheckCollision(owner->GetFrontObject(ObjectType::Neutral)->GetComponent<CollisionComponent>());
    if (!backgroundCollisionCheck)
    {                  
        isDead = true;
    }

    switch (objType)
    {
    case ObjectType::PBullet:
        {
            auto monsters = owner->GetObjectList(ObjectType::Enemy);
            for (auto& monster : monsters)
            {
                bool monsterCollisionCheck = collision->CheckCollision(monster->GetComponent<CollisionComponent>());
                if (monsterCollisionCheck)
                {
                    isDead = true;
                    monster->SetDead();
                }
            }
        }
        break;
    case ObjectType::EBullet:
        {
            auto player = owner->GetFrontObject(ObjectType::Player);
            bool playerCollisionCheck = collision->CheckCollision(player->GetComponent<CollisionComponent>());
            if (playerCollisionCheck)
            {
                isDead = true;
                player->SetDead();
            } 
        }
        break;
    }
}
