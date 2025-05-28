#include "pch.h"
#include "GameScene2.h"
#include "CWoodMainBacks.h"

void GameScene2::Load()
{
	Scene::Load();
	objectManager->AddObject(ObjectType::Front, std::make_shared<CWoodMainBacks>(objectManager.get(), ObjectType::Front, 100.));
}

void GameScene2::Update(double dt)
{
	objectManager->Update(dt);
}

void GameScene2::Render(HDC hdc)
{
	objectManager->Render(hdc);


	HPEN linePen = CreatePen(PS_SOLID, 4, RGB(61, 38, 27));
	HPEN oldPen;
	HBRUSH backBrush = CreateSolidBrush( RGB(84, 52, 38));
	HBRUSH oldBrush;
	int leftx = 470;
	int topy = 200;
	oldPen = (HPEN)SelectObject(hdc, linePen);
	oldBrush = (HBRUSH)SelectObject(hdc, backBrush);
	Rectangle(hdc, leftx, topy, leftx + 140, topy + 140);
	Rectangle(hdc, leftx+160, topy, leftx + 140 + 160, topy + 140);
	Rectangle(hdc, leftx, topy+160, leftx + 140, topy + 140 + 160);
	Rectangle(hdc, leftx + 160, topy + 160, leftx + 140 + 160, topy + 140 + 160);
	SelectObject(hdc, oldBrush);
	SelectObject(hdc, oldPen);

	DeleteObject(linePen);
	DeleteObject(backBrush);
}

void GameScene2::Unload()
{
}
