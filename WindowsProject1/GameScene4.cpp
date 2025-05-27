#include "pch.h"
#include "GameScene4.h"

void GameScene4::Load()
{
	Scene::Load();
}

void GameScene4::Update(double dt)
{
	objectManager->Update(dt);
}

void GameScene4::Render(HDC hdc)
{
	objectManager->Render(hdc);
}

void GameScene4::Unload()
{
}
