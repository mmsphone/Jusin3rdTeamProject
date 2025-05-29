#include "pch.h"
#include "GameScene2.h"
#include "CWoodMainBacks.h"
#include "Engine.h"
#include "TextureManager.h"
#include "CWoodBlockCursor.h"
#include "CWoodBlockButtons.h"
#include "CWoodBlockShape.h"
#include "CWoodEndUI.h"
#include "CWoodEndButton.h"
#include "ButtonObject.h"
#include "SceneManager.h"

void GameScene2::Load()
{
	Scene::Load();

	// objectManager->AddObject(ObjectType::Front, std::make_shared<CWoodEndUI>(objectManager.get(), ObjectType::Front, 100.));

	objectManager->AddObject(ObjectType::Back, std::make_shared<CWoodMainBacks>(objectManager.get(), ObjectType::Back, 100.));
	objectManager->AddObject(ObjectType::Cursor, std::make_shared<CWoodBlockCursor>(objectManager.get(), ObjectType::Cursor, 100.));
	
	auto* pTextureMgr = Engine::GetInstance().GetTextureManager();
	pTextureMgr->InsertTexture(L"../image/game2Back.bmp", L"game2Back", 800.f, 637.f);

	//1
	objectManager->AddObject(ObjectType::UI, std::make_shared<CWoodBlockButtons>(objectManager.get(), ObjectType::UI, 100.));

}

void GameScene2::Update(double dt)
{
	if (!bTimeOver) {
		elapsedTime += dt;
		objectManager->Update(dt);

		int remainingTime = static_cast<int>(totalTime - elapsedTime);
		if (remainingTime < 0) remainingTime = 0;

		min = remainingTime / 60;
		sec = remainingTime % 60;

		if (elapsedTime >= totalTime) {
			if (!bTimeOver) {
				objectManager->AddObject(ObjectType::Front, std::make_shared<CWoodEndUI>(objectManager.get(), ObjectType::Front, 100.));
				std::dynamic_pointer_cast<CWoodEndUI>(objectManager->GetObjectList(ObjectType::Front).front())->score = std::dynamic_pointer_cast<CWoodMainBacks>(objectManager->GetObjectList(ObjectType::Back).front())->iScore;


				std::wstring name = L"Menu";

				if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {

					Engine::GetInstance().sceneManager->SetActiveScene(std::string(name.begin(), name.end()));
				}
			}
			bTimeOver = true;
			elapsedTime = totalTime;
			std::dynamic_pointer_cast<CWoodBlockCursor>(objectManager->GetObjectList(ObjectType::Cursor).front())->bshow = true;
			// TODO: 스코어 정산 화면으로 전환
		}
	}
	else {
		
		objectManager->GetObjectList(ObjectType::Cursor).front()->Update(dt);
		objectManager->GetObjectList(ObjectType::Front).front()->Update(dt);
	}
	
}

void GameScene2::Render(HDC hdc)
{

	auto* pTextureMgr = Engine::GetInstance().GetTextureManager();
	HDC hFrameDC = pTextureMgr->FindTexture(L"game2Back");
	// if (!hFrameDC) return;

	BitBlt(hdc,0,0,800,637,hFrameDC,0, 0,SRCCOPY);

	HPEN linePen = CreatePen(PS_SOLID, 4, RGB(61, 38, 27));
	HPEN linePen2 = CreatePen(PS_SOLID, 4, RGB(191, 157, 119));
	HPEN oldPen;
	HBRUSH backBrush = CreateSolidBrush( RGB(84, 52, 38));
	HBRUSH backBrush2 = CreateSolidBrush(RGB(191, 157, 119));
	HBRUSH oldBrush;


	oldPen = (HPEN)SelectObject(hdc, linePen2);
	oldBrush = (HBRUSH)SelectObject(hdc, backBrush2);
	Rectangle(hdc, 470, 160, 770, 230);
	SelectObject(hdc, oldBrush);
	SelectObject(hdc, oldPen);


	int leftx = 470;
	int topy = 250;
	oldPen = (HPEN)SelectObject(hdc, linePen);
	oldBrush = (HBRUSH)SelectObject(hdc, backBrush);
	Rectangle(hdc, 130, 60, 370, 110);

	// Rectangle(hdc, leftx, topy, leftx + 140, topy + 140);
	// Rectangle(hdc, leftx+160, topy, leftx + 140 + 160, topy + 140);
	// Rectangle(hdc, leftx, topy+160, leftx + 140, topy + 140 + 160);
	// Rectangle(hdc, leftx + 160, topy + 160, leftx + 140 + 160, topy + 140 + 160);
	SelectObject(hdc, oldBrush);
	SelectObject(hdc, oldPen);



	wchar_t szScoreText[64];
	wchar_t szTimeText[64];
	wsprintf(szTimeText, L"Time : %02d:%02d", min, sec);
	wsprintf(szScoreText, L"Score : %d", std::dynamic_pointer_cast<CWoodMainBacks>(objectManager->GetObjectList(ObjectType::Back).front())->iScore);


	SetBkMode(hdc, TRANSPARENT);
	SetTextColor(hdc, RGB(255, 255, 255));

	HFONT hFont = CreateFont(
		35, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
		HANGUL_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
		L"맑은 고딕"
	);

	HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);

	SIZE textSize;
	GetTextExtentPoint32(hdc, szScoreText, lstrlen(szScoreText), &textSize);
	int textX = 470 + (300 - textSize.cx) / 2;
	int textY = 155 + (70 - textSize.cy) / 2;
	TextOut(hdc, textX, textY, szScoreText, lstrlen(szScoreText));


	GetTextExtentPoint32(hdc, szTimeText, lstrlen(szTimeText), &textSize);
	textX = 130 + (240 - textSize.cx) / 2;
	textY = 55 + (50 - textSize.cy) / 2;
	TextOut(hdc, textX, textY, szTimeText, lstrlen(szTimeText));

	// 리소스 정리
	SelectObject(hdc, hOldFont);
	DeleteObject(hFont);


	DeleteObject(linePen);
	DeleteObject(backBrush);
	DeleteObject(linePen2);
	DeleteObject(backBrush2);


	if (!bTimeOver) {
		objectManager->Render(hdc);

	}
	else {

		objectManager->GetObjectList(ObjectType::Cursor).front()->Render(hdc);
		objectManager->GetObjectList(ObjectType::Front).front()->Render(hdc);
	}
}

void GameScene2::Unload()
{
}
