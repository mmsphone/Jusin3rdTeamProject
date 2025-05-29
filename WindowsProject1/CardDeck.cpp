#include "pch.h"
#include "CardDeck.h"
#include "TransformComponent.h"

CardDeck::CardDeck(ObjectManager* pOwner)
    : Object(pOwner, ObjectType::Back) {
    AddComponent<TransformComponent>();
}

void CardDeck::Render(HDC hdc) {
    auto pTransform = GetComponent<TransformComponent>();
    if (!pTransform) return;

    D3DXVECTOR3 vPos = pTransform->GetPosition();
    D3DXVECTOR3 vScale = pTransform->GetScale();

    RECT rc = {
        (int)(vPos.x - vScale.x * 0.5f),
        (int)(vPos.y - vScale.y * 0.5f),
        (int)(vPos.x + vScale.x * 0.5f),
        (int)(vPos.y + vScale.y * 0.5f)
    };

    Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom); // 카드 뒷면처럼 출력
}
