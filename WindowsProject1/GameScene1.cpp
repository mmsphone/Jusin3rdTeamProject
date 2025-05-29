#include "pch.h"

#include "Scene.h"
#include "GameScene1.h"
#include "ObjectManager.h"
#include "TextureManager.h"

#include "BigWheel.h"
#include "ButtonYu.h"

void GameScene1::Load() 
{
	Scene::Load();
	pCoin = nullptr;
	objectManager->AddObject(ObjectType::Neutral, std::make_shared<BigWheel>(objectManager.get(), ObjectType::Neutral));

	if (!pCoin)
	{
		pCoin = new int;
		*pCoin = 100;
	}
	for (UINT eID = 0; eID < BUTTON_END; ++eID) {
		BUTTON_ID ButtonNum = static_cast<BUTTON_ID>(eID);
		objectManager->AddObject(ObjectType::Back, std::make_shared<ButtonYu>(objectManager.get(), ObjectType::Back, ButtonNum, pCoin));
		
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
{
	if (pCoin) { delete pCoin; pCoin = nullptr; }
}

