#include "pch.h"
#include "CWoodMainBacks.h"
#include "TransformComponent.h"
#include "CollisionComponent.h"
#include "Engine.h"
#include "InputSystem.h"
#include "ObjectManager.h"
#include "CWoodBlockBack.h"

CWoodMainBacks::CWoodMainBacks(ObjectManager* owner, ObjectType objType, double speed)
    : Object(owner, objType, RenderType::Rect), speed(speed)
{
    auto transform = AddComponent<TransformComponent>();
    transform->SetPosition(70.0f, 120.0f);
    transform->SetScale(400.0f, 400.0f);

    auto collision = AddComponent<CollisionComponent>();
    collision->Initialize();

    for (int i = 0; i < iGridH; ++i) {
        for (int j = 0; j < iGridW; ++j) {
            float fX = (j*40)+ transform->GetPosition().x;
            float fY = (i*40) + transform->GetPosition().y;
            std::shared_ptr<Object> blockTile = std::make_shared<CWoodBlockBack>(owner, ObjectType::Front, 100.);

            blockTile->GetComponent<TransformComponent>()->SetPosition(fX, fY);

            // blocks.push_back({ false,std::shared_ptr<Object>(std::make_shared<CWoodMainBacks>(owner, ObjectType::Front, 100.))});
            blocks.push_back({ false,false, blockTile });
        }
    }

    // objectManager = std::make_unique<ObjectManager>();
    // objectManager->Initialize();
}

void CWoodMainBacks::Update(double dt) {

}

void CWoodMainBacks::Render(HDC hdc) {

    HPEN linePen = CreatePen(PS_SOLID, 6, RGB(61, 38, 27));
    HPEN oldPen;

    auto transform = GetComponent<TransformComponent>();
    if (!transform) return; // 없으면 그리지 않음

    D3DXVECTOR3 pos = transform->GetPosition();
    D3DXVECTOR3 scale = transform->GetScale();
    float rotateZ = transform->GetRotationZ();

    float halfW = scale.x * 0.51f;
    float halfH = scale.y * 0.51f;
    int cx = static_cast<int>(pos.x+180);
    int cy = static_cast<int>(pos.y+180);

    float cosTheta = cosf(rotateZ);
    float sinTheta = sinf(rotateZ);

    POINT points[4];

    D3DXVECTOR3 localCorners[4] = {
        { -halfW, -halfH, 0 }, // LT
        {  halfW, -halfH, 0 }, // RT
        {  halfW,  halfH, 0 }, // RB
        { -halfW,  halfH, 0 }  // LB
    };

    for (int i = 0; i < 4; ++i) {
        float x = localCorners[i].x;
        float y = localCorners[i].y;

        // 회전 적용
        float rx = x * cosTheta - y * sinTheta;
        float ry = x * sinTheta + y * cosTheta;

        points[i].x = static_cast<LONG>(cx + rx);
        points[i].y = static_cast<LONG>(cy + ry);

    }

    oldPen = (HPEN)SelectObject(hdc, linePen);
    Polygon(hdc, points, 4);
    SelectObject(hdc, oldPen);

    DeleteObject(linePen);

    for (auto& block : blocks) {
        block.block->Render(hdc);
    }


}