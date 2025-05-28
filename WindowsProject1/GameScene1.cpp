#include "pch.h"
#include "Scene.h"
#include "GameScene1.h"
#include "ObjectManager.h"
#include "TextureManager.h"

#include "BigWheel.h"
#include "ButtonYu.h"
#include "NoteYu.h"

void GameScene1::Load()
{
	Scene::Load();
	//objectManager->AddObject(ObjectType::Neutral, std::make_shared<BigWheel>(objectManager.get(), ObjectType::Neutral));
	//objectManager->AddObject(ObjectType::Mid, std::make_shared<Player>(objectManager.get(), ObjectType::Mid, 100.));
	
	for (UINT eID = 0; eID < BUTTON_END; ++eID) {
		BUTTON_ID ButtonNum = static_cast<BUTTON_ID>(eID);
		objectManager->AddObject(ObjectType::Back, std::make_shared<ButtonYu>(objectManager.get(), ObjectType::Back, ButtonNum));
	}
	for (UINT eID = 0; eID < BUTTON_END; ++eID) {
		BUTTON_ID NoteNum = static_cast<BUTTON_ID>(eID);
		objectManager->AddObject(ObjectType::Mid, std::make_shared<NoteYu>(objectManager.get(), ObjectType::Mid, NoteNum,500));
	}

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
