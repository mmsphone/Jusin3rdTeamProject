#include "pch.h"
#include "CWoodBlockButton.h"
#include "CWoodBlockShape.h"

CWoodBlockButton::CWoodBlockButton(ObjectManager* owner, ObjectType objType, double speed, int _iBlockType)
    : Object(owner, objType, RenderType::Rect), speed(speed)
{
    iBlockType = _iBlockType;

    auto transform = AddComponent<TransformComponent>();
    transform->SetPosition(70.0f, 170.0f);
    transform->SetScale(140.0f, 140.0f);

    blockshape = std::make_shared<CWoodBlockShape>(owner, ObjectType::Mid, 100., iBlockType);



}
void CWoodBlockButton::Update(double dt) {
    

    auto transform = GetComponent<TransformComponent>();
    if (!transform) return;
   
    auto shapeTransform = blockshape->GetComponent<TransformComponent>();
    if (shapeTransform) {
        shapeTransform->SetPosition(transform->GetPosition().x+40, transform->GetPosition().y + 40);
    }

    D3DXVECTOR3 pos = transform->GetPosition();
    D3DXVECTOR3 scale = transform->GetScale();

    POINT	ptMouse{};
    GetCursorPos(&ptMouse);
    ScreenToClient(Engine::GetInstance().GethWnd(), &ptMouse);

    RECT rect = {
        (LONG)pos.x,
        (LONG)pos.y,
        (LONG)(pos.x + scale.x),
        (LONG)(pos.y + scale.y)
    };

    auto input = Engine::GetInstance().GetInputSystem();
    if (PtInRect(&rect, ptMouse))
    {
        if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
            if (!isSelected && !isBlockin) {
                wasJustClicked = true; // 클릭되었음을 표시
            }
        }

        bHover = true;
    }
    else {
        bHover = false;
    }

    blockshape->Update(dt);
}

void CWoodBlockButton::Render(HDC hdc) {

    HPEN linePen = CreatePen(PS_SOLID, 4, RGB(61, 38, 27));
    HPEN oldPen;
    HBRUSH backBrush = CreateSolidBrush((isBlockin? RGB(42, 20, 16) :(isSelected ? RGB(145, 90, 65) : (bHover ? RGB(66, 41, 30) : RGB(84, 52, 38)))));
    HBRUSH oldBrush;

    auto transform = GetComponent<TransformComponent>();
    if (!transform) return;

    D3DXVECTOR3 pos = transform->GetPosition();
    D3DXVECTOR3 scale = transform->GetScale();

    oldPen = (HPEN)SelectObject(hdc, linePen);
    oldBrush = (HBRUSH)SelectObject(hdc, backBrush);
    Rectangle(hdc, (int)pos.x, (int)pos.y, (int)(pos.x + scale.x), (int)(pos.y + scale.y));
    SelectObject(hdc, oldBrush);
    SelectObject(hdc, oldPen);


    DeleteObject(linePen);
    DeleteObject(backBrush);

    if (!isBlockin) {
        blockshape->Render(hdc);
    }
}