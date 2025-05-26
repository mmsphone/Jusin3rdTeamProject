#include "pch.h"
#include "MenuScene.h"
#include "ButtonObject.h"
#include "Engine.h"
#include "SceneManager.h"
#include "InputSystem.h"

void MenuScene::Load() {
    Scene::Load();
    CreateMainMenu();
}

void MenuScene::Unload() {
}

void MenuScene::Update(double dt) {
    objectManager->Update(dt);
}

void MenuScene::CreateMainMenu() {
    eMenuState = MenuState::MAIN;
    objectManager->Initialize(); // 버튼 리셋

    auto startBtn = std::make_shared<ButtonObject>(
        objectManager.get(), L"START",
        [this]() { CreateGameSelectMenu(); }
    );
    auto tfStart = startBtn->AddComponent<TransformComponent>();
    tfStart->SetPosition(400, 350);
    tfStart->SetScale(200, 50);
    objectManager->AddObject(ObjectType::UI, startBtn);

    auto exitBtn = std::make_shared<ButtonObject>(
        objectManager.get(), L"EXIT",
        []() { Engine::GetInstance().Shutdown(); }
    );
    auto tfExit = exitBtn->AddComponent<TransformComponent>();
    tfExit->SetPosition(400, 420);
    tfExit->SetScale(200, 50);
    objectManager->AddObject(ObjectType::UI, exitBtn);
}

void MenuScene::CreateGameSelectMenu() {
    eMenuState = MenuState::SELECT;

    //objectManager->ClearObjectList(ObjectType::UI);

    const std::vector<std::wstring> gameNames = { L"Game1", L"Game2", L"Game3", L"Game4" };
    int startY = 280;
    int gapY = 60;

    for (int i = 0; i < 4; ++i) {
        std::wstring name = gameNames[i];

        auto btn = std::make_shared<ButtonObject>(
            objectManager.get(), name,
            [name]() {
                Engine::GetInstance().sceneManager->SetActiveScene(std::string(name.begin(), name.end()));
            }
        );
        auto tf = btn->AddComponent<TransformComponent>();
        tf->SetPosition(400, startY + i * gapY);
        tf->SetScale(200, 50);
        objectManager->AddObject(ObjectType::UI, btn);
    }
}


void MenuScene::Render(HDC hdc) {
    // 타이틀
    SetBkMode(hdc, TRANSPARENT);
    HFONT hFont = CreateFontW(48, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        HANGEUL_CHARSET, OUT_TT_ONLY_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"맑은 고딕");
    HFONT hOld = (HFONT)SelectObject(hdc, hFont);
    DrawTextW(hdc, L"My Game Title", -1, new RECT{ 0, 50, 800, 150 },
        DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    SelectObject(hdc, hOld);
    DeleteObject(hFont);

    objectManager->Render(hdc);
}
