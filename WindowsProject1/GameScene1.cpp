#include "pch.h"
#include "Scene.h"
#include "GameScene1.h"
#include "ObjectManager.h"
#include "Player.h"
#include "Background.h"
#include "Boss.h"
void GameScene1::Load()
{
	Scene::Load();
	objectManager->AddObject(ObjectType::Neutral, std::make_shared<Background>(objectManager.get(), ObjectType::Neutral));
	objectManager->AddObject(ObjectType::Player, std::make_shared<Player>(objectManager.get(), ObjectType::Player, 100.));
	objectManager->AddObject(ObjectType::Enemy, std::make_shared<Boss>(objectManager.get(), ObjectType::Enemy, 100.));
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
