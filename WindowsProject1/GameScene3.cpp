#include "pch.h"
#include "GameScene3.h"

void GameScene3::Load()
{
	Scene::Load();
}

void GameScene3::Update(double dt)
{
	objectManager->Update(dt);
}

void GameScene3::Render(HDC hdc)
{
	objectManager->Render(hdc);
}

void GameScene3::Unload()
{
}
