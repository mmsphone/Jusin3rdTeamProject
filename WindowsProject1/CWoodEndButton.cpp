#include "pch.h"
#include "CWoodEndButton.h"
#include "CWoodBlockShape.h"
#include "Engine.h"
#include "SceneManager.h"
#include "CWoodBlockCursor.h"


CWoodEndButton::CWoodEndButton(ObjectManager* owner, ObjectType objType, double speed)
    : Object(owner, objType, RenderType::Rect), speed(speed)
{

    auto transform = AddComponent<TransformComponent>();
    transform->SetPosition(330.0f, 260.0f);
    transform->SetScale(155.0f, 50.0f);
}
void CWoodEndButton::Update(double dt) {


    auto transform = GetComponent<TransformComponent>();
    if (!transform) return;
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


        std::wstring name = L"Menu";

        if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
            // Engine::GetInstance().sceneManager->SetActiveScene("Clear");
            Engine::GetInstance().sceneManager->SetActiveScene(std::string(name.begin(), name.end()));
        }
    

        bHover = true;
    }
    else {
        bHover = false;
    }

}

void CWoodEndButton::Render(HDC hdc) {

    HPEN linePen = CreatePen(PS_SOLID, 4, RGB(61, 38, 27));
    HPEN oldPen;
    HBRUSH backBrush = CreateSolidBrush((bHover ? RGB(66, 41, 30) : RGB(84, 52, 38)));
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

}