#include "pch.h"
#include "Scene.h"
#include "GameScene1.h"
#include "ObjectManager.h"
#include "BigWheel.h"
void GameScene1::Load()
{
	Scene::Load();
	objectManager->AddObject(ObjectType::Neutral, std::make_shared<BigWheel>(objectManager.get(), ObjectType::Neutral));

	//objectManager->AddObject(ObjectType::Mid, std::make_shared<Player>(objectManager.get(), ObjectType::Mid, 100.));

}

void GameScene1::Update(double dt)
{
	objectManager->Update(dt);
}

void GameScene1::Render(HDC hdc)
{
	objectManager->Render(hdc);
}

void GameScene1::Unload()
{}
