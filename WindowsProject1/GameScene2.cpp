#include "pch.h"
#include "GameScene2.h"
#include "CWoodMainBacks.h"
#include "Engine.h"
#include "TextureManager.h"
#include "CWoodBlockCursor.h"

void GameScene2::Load()
{
	Scene::Load();
	objectManager->AddObject(ObjectType::Front, std::make_shared<CWoodMainBacks>(objectManager.get(), ObjectType::Front, 100.));
	objectManager->AddObject(ObjectType::Cursor, std::make_shared<CWoodBlockCursor>(objectManager.get(), ObjectType::Cursor, 100.));
	
	auto* pTextureMgr = Engine::GetInstance().GetTextureManager();
	pTextureMgr->InsertTexture(L"../image/game2Back.bmp", L"game2Back", 800.f, 637.f);
}

void GameScene2::Update(double dt)
{
	objectManager->Update(dt);
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

	Rectangle(hdc, leftx, topy, leftx + 140, topy + 140);
	Rectangle(hdc, leftx+160, topy, leftx + 140 + 160, topy + 140);
	Rectangle(hdc, leftx, topy+160, leftx + 140, topy + 140 + 160);
	Rectangle(hdc, leftx + 160, topy + 160, leftx + 140 + 160, topy + 140 + 160);
	SelectObject(hdc, oldBrush);
	SelectObject(hdc, oldPen);



	wchar_t szScoreText[64];
	wchar_t szTimeText[64];
	wsprintf(szTimeText, L"Time : %02d:%02d", 0, 50);
	wsprintf(szScoreText, L"Score : %d", 0);

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


	objectManager->Render(hdc);

}

void GameScene2::Unload()
{
}
