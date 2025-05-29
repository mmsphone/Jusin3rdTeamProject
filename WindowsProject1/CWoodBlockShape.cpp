#include "pch.h"
#include "CWoodBlockShape.h"
#include "TransformComponent.h"
#include "CollisionComponent.h"
#include "Engine.h"
#include "InputSystem.h"
#include "ObjectManager.h"
#include "CWoodBlock.h"

CWoodBlockShape::CWoodBlockShape(ObjectManager* owner, ObjectType objType, double speed, int _iBlockType)
    : Object(owner, objType, RenderType::Rect), speed(speed)
{
    iBlockType = _iBlockType;

    auto transform = AddComponent<TransformComponent>();
    transform->SetPosition(70.0f, 170.0f);
    transform->SetScale(120.0f, 120.0f);

    auto collision = AddComponent<CollisionComponent>();
    collision->Initialize();

    if (objType == ObjectType::Mid) {
        fWoodBlockSize = 30.f;
    }

    D3DXVECTOR3 centerPos = transform->GetPosition() + D3DXVECTOR3(fWoodBlockSize, fWoodBlockSize, 0.0f);


    const auto& shape = blockShapes[iBlockType];

    for (int i = 0; i < shape.size(); ++i) {
        for (int j = 0; j < shape[i].size(); ++j) {
            if (!shape[i][j]) continue;

            float localX = j * fWoodBlockSize;
            float localY = i * fWoodBlockSize;

            D3DXVECTOR3 offset(localX - fWoodBlockSize, localY - fWoodBlockSize, 0);
            std::shared_ptr<Object> blockTile = std::make_shared<CWoodBlock>(owner, objType, 100.);
            blockTile->GetComponent<TransformComponent>()->SetPosition(centerPos.x + offset.x, centerPos.y + offset.y);
            blockTile->GetComponent<TransformComponent>()->SetScale(fWoodBlockSize, fWoodBlockSize);
            blocks.push_back({ blockTile, offset });
        }
    }
}

void CWoodBlockShape::Update(double dt) {

    auto transform = GetComponent<TransformComponent>();
    if (!transform) return;
    
    // auto input = Engine::GetInstance().GetInputSystem();
    // if (input->IsKeyDown('R')) {
    // 	transform->SetRotationZ(transform->GetRotationZ() + D3DXToRadian(3.f));
    // }

    if (isRotating) {
        float angleDiff = targetAngle - currentAngle;
        float angleStep = D3DXToRadian(180.0f) * static_cast<float>(dt); // √ ¥Á 180µµ

        if (fabs(angleDiff) <= angleStep) {
            currentAngle = targetAngle;
            isRotating = false;
        }
        else {
            currentAngle += (angleDiff > 0 ? 1 : -1) * angleStep;
        }

        transform->SetRotationZ(currentAngle);
    }


    float angle = transform->GetRotationZ();
    D3DXMATRIX rotMatrix;
    D3DXMatrixRotationZ(&rotMatrix, transform->GetRotationZ());
    D3DXVECTOR3 centerPos = transform->GetPosition() + D3DXVECTOR3(fWoodBlockSize, fWoodBlockSize, 0.0f);

    for (auto& block : blocks) {
        D3DXVECTOR3 rotatedOffset;
        D3DXVec3TransformCoord(&rotatedOffset, &block.relativeOffset, &rotMatrix);
        D3DXVECTOR3 newPos(centerPos.x + rotatedOffset.x, centerPos.y + rotatedOffset.y, 0.f);
        block.block->GetComponent<TransformComponent>()->SetPosition(newPos.x,newPos.y);
        block.block->GetComponent<TransformComponent>()->SetRotationZ(transform->GetRotationZ());
        block.block->Update(dt);
    }
}

void CWoodBlockShape::Render(HDC hdc) {


    for (auto& block : blocks) {
        block.block->Render(hdc);
    }


}


void CWoodBlockShape::RequestRotate() {
    if (!isRotating) {
        targetAngle += D3DXToRadian(90.0f);
        isRotating = true;
    }
}