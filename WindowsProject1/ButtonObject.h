#pragma once
#include "Object.h"
#include "Engine.h"
#include "InputSystem.h"
#include "TransformComponent.h"

class ButtonObject : public Object {
public:
    std::wstring text;
    std::function<void()> onClick;

private:
    bool m_bVisible = true;

public:
    ButtonObject(ObjectManager* pOwner, std::wstring label, std::function<void()> func)
        : Object(pOwner, ObjectType::UI, RenderType::Rect),
        text(std::move(label)), onClick(std::move(func)) {
        AddComponent<TransformComponent>();
    }

    void SetVisible(bool bVisible) { m_bVisible = bVisible; }
    bool IsVisible() const { return m_bVisible; }

    void Update(double dt) override {
        if (!m_bVisible) return;

        auto input = Engine::GetInstance().GetInputSystem();
        if (input->IsMousePressed()) {
            auto tf = GetComponent<TransformComponent>();
            if (!tf) return;

            D3DXVECTOR3 pos = tf->GetPosition();
            D3DXVECTOR3 scale = tf->GetScale();

            RECT rc = {
                (LONG)(pos.x - scale.x * 0.5f),
                (LONG)(pos.y - scale.y * 0.5f),
                (LONG)(pos.x + scale.x * 0.5f),
                (LONG)(pos.y + scale.y * 0.5f)
            };

            D3DXVECTOR3 vMouse = input->GetMousePos();
            POINT pt = { (LONG)vMouse.x, (LONG)vMouse.y };

            if (PtInRect(&rc, pt) && onClick) {
                onClick();
            }
        }
    }

    void Render(HDC hdc) override {
        if (!m_bVisible) return;

        auto tf = GetComponent<TransformComponent>();
        if (!tf) return;

        D3DXVECTOR3 pos = tf->GetPosition();
        D3DXVECTOR3 scale = tf->GetScale();

        RECT rc = {
            (LONG)(pos.x - scale.x * 0.5f),
            (LONG)(pos.y - scale.y * 0.5f),
            (LONG)(pos.x + scale.x * 0.5f),
            (LONG)(pos.y + scale.y * 0.5f)
        };

        Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
        DrawTextW(hdc, text.c_str(), -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }
};
