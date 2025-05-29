#include "pch.h"
#include "CWoodMainBacks.h"
#include "TransformComponent.h"
#include "CollisionComponent.h"
#include "Engine.h"
#include "InputSystem.h"
#include "ObjectManager.h"
#include "CWoodBlockBack.h"
#include "CWoodBlockButtons.h"

CWoodMainBacks::CWoodMainBacks(ObjectManager* owner, ObjectType objType, double speed)
    : Object(owner, objType, RenderType::Rect), speed(speed)
{
    auto transform = AddComponent<TransformComponent>();
    transform->SetPosition(70.0f, 170.0f);
    transform->SetScale(400.0f, 400.0f);

    auto collision = AddComponent<CollisionComponent>();
    collision->Initialize();

    for (int i = 0; i < iGridH; ++i) {
        for (int j = 0; j < iGridW; ++j) {
            float fX = (j*40)+ transform->GetPosition().x;
            float fY = (i*40) + transform->GetPosition().y;
            std::shared_ptr<Object> blockTile = std::make_shared<CWoodBlockBack>(owner, objType, 100.);

            blockTile->GetComponent<TransformComponent>()->SetPosition(fX, fY);

            // blocks.push_back({ false,std::shared_ptr<Object>(std::make_shared<CWoodMainBacks>(owner, ObjectType::Front, 100.))});
            blocks.push_back({ false,false, blockTile });
        }
    }

    // objectManager = std::make_unique<ObjectManager>();
    // objectManager->Initialize();
}

std::vector<std::vector<bool>> CWoodMainBacks::RotateShape(int type, float radians) {

// #ifdef _DEBUG
//     OutputDebugStringW((L"ȸ��: " + radians + L"\n").c_str());
// #endif
    int angle = (int)(radians) % 360;
    if (angle < 0) angle += 360;

    const std::vector<std::vector<bool>>& shape = blockShapes[type];
    std::vector<std::vector<bool>> result(3, std::vector<bool>(3, false));

    switch (angle) {
    case 90:
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                result[j][2 - i] = shape[i][j]; // �ð� ���� ȸ��
        break;

    case 180:
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                result[2 - i][2 - j] = shape[i][j]; // 180�� ȸ��
        break;

    case 270:
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                result[2 - j][i] = shape[i][j]; // �ݽð� ���� 90�� = �ð���� 270��
        break;

    default:
        result = shape; // 0�� ȸ�� (���� �״��)
        break;
    }

    return result;
}
void CWoodMainBacks::Update(double dt) {
    auto buttonUI = std::dynamic_pointer_cast<CWoodBlockButtons>(owner->GetObjectList(ObjectType::UI).front());
    int iBlockType = buttonUI->iBlockType;
    std::shared_ptr<CWoodBlockShape> blockShape = buttonUI->blockshape;

    // ��� ����� �浹 �ð�ȭ �ʱ�ȭ
    for (auto& block : blocks) {
        std::dynamic_pointer_cast<CWoodBlockBack>(block.block)->b_CllisionCheak = false;
    }

    if (iBlockType != -1) {
        POINT mousePos;
        GetCursorPos(&mousePos);
        ScreenToClient(GetActiveWindow(), &mousePos);

        bool foundCenter = false;
        int baseX = 0, baseY = 0;

        // ���콺 ��ġ ��� �߽� �� ã��
        for (int i = 0; i < iGridH && !foundCenter; ++i) {
            for (int j = 0; j < iGridW; ++j) {
                int index = i * iGridW + j;
                auto transform = blocks[index].block->GetComponent<TransformComponent>();
                if (!transform) continue;

                D3DXVECTOR3 pos = transform->GetPosition();
                RECT rect = { (LONG)pos.x, (LONG)pos.y, (LONG)(pos.x + 40), (LONG)(pos.y + 40) };

                if (PtInRect(&rect, mousePos)) {
                    baseX = j;
                    baseY = i;
                    foundCenter = true;
                    break;
                }
            }
        }

        if (foundCenter) {
            std::vector<std::vector<bool>> rotatedShape = RotateShape(iBlockType, buttonUI->angle);

            const int centerX = 1;
            const int centerY = 1;

            std::vector<int> validIndices;
            bool canPlace = true;

            for (int y = 0; y < 3; ++y) {
                for (int x = 0; x < 3; ++x) {
                    if (!rotatedShape[y][x]) continue;

                    int offsetX = x - centerX;
                    int offsetY = y - centerY;

                    int targetX = baseX + offsetX;
                    int targetY = baseY + offsetY;

                    if (targetX < 0 || targetX >= iGridW || targetY < 0 || targetY >= iGridH) {
                        canPlace = false;
                        continue;
                    }

                    int tIndex = targetY * iGridW + targetX;

                    if (std::dynamic_pointer_cast<CWoodBlockBack>(blocks[tIndex].block)->bBlockCheak) {
                        canPlace = false;
                    }

                    validIndices.push_back(tIndex);
                }
            }

            // ���콺 Ŭ�� �� ��� ��ġ
            if (canPlace && (GetAsyncKeyState(VK_LBUTTON) & 0x8000)) {
                for (int idx : validIndices) {
                    std::dynamic_pointer_cast<CWoodBlockBack>(blocks[idx].block)->bBlockCheak = true;
                    ++iScore;
                }
                buttonUI->b_blockIn = true;
            }

            // �浹 �ð�ȭ
            for (int idx : validIndices) {
                std::dynamic_pointer_cast<CWoodBlockBack>(blocks[idx].block)->b_CllisionCheak = true;
            }
        }
    }

    // ��� �� ������Ʈ
    for (auto& block : blocks) {
        block.block->Update(dt);
    }
}



void CWoodMainBacks::Render(HDC hdc) {

    HPEN linePen = CreatePen(PS_SOLID, 6, RGB(61, 38, 27));
    HPEN oldPen;

    auto transform = GetComponent<TransformComponent>();
    if (!transform) return; // ������ �׸��� ����

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

        // ȸ�� ����
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