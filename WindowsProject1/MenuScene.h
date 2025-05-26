#pragma once
#include "Scene.h"

class MenuScene : public Scene {
public:
    virtual void Load() override;
    virtual void Unload() override;
    virtual void Render(HDC hdc) override;
    virtual void Update(double dt) override;

private:
    enum class MenuState {
        MAIN,
        SELECT
    } 
    eMenuState = MenuState::MAIN;

    void CreateMainMenu();
    void CreateGameSelectMenu();
};