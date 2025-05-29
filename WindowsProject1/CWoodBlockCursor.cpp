#include "pch.h"
#include "CWoodBlockCursor.h"
#include "TransformComponent.h"
#include "CollisionComponent.h"
#include "Engine.h"
#include "InputSystem.h"
#include "ObjectManager.h"
#include "TextureManager.h"

CWoodBlockCursor::CWoodBlockCursor(ObjectManager* owner, ObjectType objType, double speed)
    : Object(owner, objType, RenderType::Rect), speed(speed)
{
    auto transform = AddComponent<TransformComponent>();
    transform->SetPosition(70.0f, 170.0f);
    transform->SetScale(15.0f, 15.0f);

    auto* pTextureMgr = Engine::GetInstance().GetTextureManager();
    pTextureMgr->InsertTexture(L"../image/WoodCuser.bmp", L"WoodCuser", 148.f, 167.f);
}

void CWoodBlockCursor::Update(double dt) {

    POINT	ptMouse{};

    GetCursorPos(&ptMouse);

    ScreenToClient(Engine::GetInstance().GethWnd(), &ptMouse);

    auto transform = GetComponent<TransformComponent>();
    if (!transform) return;

    transform->SetPosition((float)ptMouse.x, (float)ptMouse.y, 0);

    
    //ShowCursor(bshow);
}

void CWoodBlockCursor::Render(HDC hdc) {

    auto transform = GetComponent<TransformComponent>();
    if (!transform) return; // 없으면 그리지 않음

    D3DXVECTOR3 pos = transform->GetPosition();
    D3DXVECTOR3 scale = transform->GetScale();

    float halfW = scale.x * 0.51f;
    float halfH = scale.y * 0.51f;

    // Ellipse(hdc, pos.x - halfW, pos.y - halfH, pos.x + halfW, pos.y + halfH);

    auto* pTextureMgr = Engine::GetInstance().GetTextureManager();
    HDC hFrameDC = pTextureMgr->FindTexture(L"WoodCuser");

    GdiTransparentBlt(hdc,
        pos.x - halfW, pos.y - halfH,
        25,40,
        hFrameDC,0, 0,
        133,152,
        RGB(75, 37, 29));

}