#include "pch.h"
#include "CWoodBlockButton.h"

CWoodBlockButton::CWoodBlockButton(ObjectManager* owner, ObjectType objType, double speed)
    : Object(owner, objType, RenderType::Rect), speed(speed)
{
    auto transform = AddComponent<TransformComponent>();
    transform->SetPosition(70.0f, 170.0f);
    transform->SetScale(140.0f, 140.0f);

}
void CWoodBlockButton::Update(double dt) {

    POINT	ptMouse{};
    GetCursorPos(&ptMouse);
    ScreenToClient(Engine::GetInstance().GethWnd(), &ptMouse);



    // if (PtInRect(&m_tRect, pt))
    // {
    // 
    // }
    // else {
    // 
    // }
}

void CWoodBlockButton::Render(HDC hdc) {

}