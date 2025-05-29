#include "pch.h"
#include "CWoodBlockButtons.h"
#include "TransformComponent.h"
#include "CollisionComponent.h"
#include "Engine.h"
#include "InputSystem.h"
#include "ObjectManager.h"
#include "CWoodBlockButton.h"

CWoodBlockButtons::CWoodBlockButtons(ObjectManager* owner, ObjectType objType, double speed)
    : Object(owner, objType, RenderType::Rect), speed(speed)
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    BlockTypeShuffle(blockTypes);

    auto transform = AddComponent<TransformComponent>();
    transform->SetPosition(70.0f, 170.0f);
    transform->SetScale(400.0f, 400.0f);

    auto collision = AddComponent<CollisionComponent>();
    collision->Initialize();

    // 버튼들 추가
    const int leftx = 470;
    const int topy = 250;
    const float btnSize = 140.f;
    const float gap = 160.f;

    for (int i = 0; i < 4; ++i) {
        int x = leftx + (i % 2) * gap;
        int y = topy + (i / 2) * gap;

        int randBlockType = rand() % 9; // 0~8 사이
        // int randBlockType = rand() % 9; // 0~8 사이

        auto button = std::make_shared<CWoodBlockButton>(owner, ObjectType::UI, 0., blockTypes[i]);
        button->GetComponent<TransformComponent>()->SetPosition(x, y);

        buttons.push_back(button);
    }
}

void CWoodBlockButtons::Update(double dt) {

    if (b_blockIn) {

        int ibuttonsblockIns = 0;

        for (auto& button : buttons) {
            auto btn = std::dynamic_pointer_cast<CWoodBlockButton>(button);
            if (btn && btn->isSelected) {
                btn->isBlockin = true;
                btn->isSelected = false;
            }
            if (btn && btn->isBlockin) {
                ++ibuttonsblockIns;
            }
        }
        angle = 0;
        blockshape.reset();
        iBlockType = -1;


        if (ibuttonsblockIns == 4) {
            BlockTypeShuffle(blockTypes);
            int i = 0;
            for (auto& button : buttons) {
                auto btn = std::dynamic_pointer_cast<CWoodBlockButton>(button);
                
                btn->isBlockin = false;

                btn->iBlockType = blockTypes[i];
                btn->blockshape.reset();
                btn->blockshape = std::make_shared<CWoodBlockShape>(owner, ObjectType::Mid, 100., blockTypes[i]);
                ++i;
            }
        }

        b_blockIn = false;
    }

    for (auto& button : buttons) {
        button->Update(dt);

        auto btn = std::dynamic_pointer_cast<CWoodBlockButton>(button);
        if (btn && btn->WasJustClicked()) {

            for (auto& other : buttons) {
                auto otherBtn = std::dynamic_pointer_cast<CWoodBlockButton>(other);
                if (otherBtn) {
                    otherBtn->isSelected = (btn == otherBtn);
                }
            }

            iBlockType = btn->iBlockType;
            btn->ClearClickFlag(); 
            angle = 0;
            blockshape.reset();
            blockshape = std::make_shared<CWoodBlockShape>(owner, ObjectType::Front, 100.0, iBlockType);


            break;
        }


    }

    if (blockshape) {

        POINT	ptMouse{};
        GetCursorPos(&ptMouse);

        auto transform = blockshape->GetComponent<TransformComponent>();
        if (transform) {
            float width = transform->GetScale().x;
            float height = transform->GetScale().y;

            transform->SetPosition(static_cast<float>(ptMouse.x-150),
                static_cast<float>(ptMouse.y - 170));
        }

        blockshape->Update(dt);
    }

    SHORT rButtonState = GetAsyncKeyState(VK_RBUTTON);
    bool isRightMouseDown = (rButtonState & 0x8000);

    if (isRightMouseDown && !wasRightMouseDown) {
        angle += 90;
        if (blockshape) {
            blockshape->RequestRotate();
        }
    }

    // auto transform = blockshape->GetComponent<TransformComponent>();
    // angle = transform->GetRotationZ();

    wasRightMouseDown = isRightMouseDown;
}

void CWoodBlockButtons::Render(HDC hdc) {

    for (auto& button : buttons) {
        button->Render(hdc);
    }

    if (blockshape) {
        blockshape->Render(hdc);
    }

}


void CWoodBlockButtons::BlockTypeShuffle(std::vector<int>& vec) {
    for (int i = vec.size() - 1; i > 0; --i) {
        int j = std::rand() % (i + 1);
        std::swap(vec[i], vec[j]);
    }
}