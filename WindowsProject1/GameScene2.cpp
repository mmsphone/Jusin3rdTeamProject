#include "pch.h"
#include "GameScene2.h"

void GameScene2::Load()
{
	Scene::Load();
}

void GameScene2::Update(double dt)
{
	objectManager->Update(dt);
}

void GameScene2::Render(HDC hdc)
{
	objectManager->Render(hdc);
}

void GameScene2::Unload()
{
}
