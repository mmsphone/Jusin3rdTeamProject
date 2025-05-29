#include "pch.h"
#include "CWoodEndUI.h"
#include "CWoodBlockShape.h"
#include "TextureManager.h"
#include "CWoodMainBacks.h"
#include "CWoodEndButton.h"

CWoodEndUI::CWoodEndUI(ObjectManager* owner, ObjectType objType, double speed)
    : Object(owner, objType, RenderType::Rect), speed(speed)
{

    auto* pTextureMgr = Engine::GetInstance().GetTextureManager();
    pTextureMgr->InsertTexture(L"../image/WoodOverBack.bmp", L"WoodOverBack", 800.f, 600.f);

    auto transform = AddComponent<TransformComponent>();
    transform->SetPosition(70.0f, 170.0f);
    transform->SetScale(140.0f, 140.0f);

	blockshape = std::make_shared<CWoodEndButton>(owner, ObjectType::Mid, 100.);


}
void CWoodEndUI::Update(double dt) {
	blockshape->Update(dt);
}

void CWoodEndUI::Render(HDC hdc) {


    auto* pTextureMgr = Engine::GetInstance().GetTextureManager();
    HDC hFrameDC = pTextureMgr->FindTexture(L"WoodOverBack");
    // if (!hFrameDC) return;

    BitBlt(hdc, 0, 0, 800, 600, hFrameDC, 0, 0, SRCCOPY);




	wchar_t szScoreText[64];
	wsprintf(szScoreText, L"%d ¡°", score);

	SetBkMode(hdc, TRANSPARENT);
	SetTextColor(hdc, RGB(255, 255, 255));

	HFONT hFont = CreateFont(
		50, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
		HANGUL_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
		L"∏º¿∫ ∞ÌµÒ"
	);

	HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);

	SIZE textSize;
	GetTextExtentPoint32(hdc, szScoreText, lstrlen(szScoreText), &textSize);
	int textX = 260 + (295 - textSize.cx) / 2;
	int textY = 110 + (90 - textSize.cy) / 2;
	TextOut(hdc, textX, textY, szScoreText, lstrlen(szScoreText));

	SelectObject(hdc, hOldFont);


	blockshape->Render(hdc);
	//Rectangle(hdc, 260+70, 210+50, 555-70, 260+50);
	
	HFONT hFont2 = CreateFont(
		30, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
		HANGUL_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
		L"∏º¿∫ ∞ÌµÒ"
	);

	wchar_t szButtonText[64];
	wsprintf(szButtonText, L"∏ﬁ¿Œ∏ﬁ¥∫");
	
	hOldFont = (HFONT)SelectObject(hdc, hFont2);

	TextOut(hdc, 370, 270, szButtonText, lstrlen(szButtonText));
	SelectObject(hdc, hOldFont);
	// ∏Æº“Ω∫ ¡§∏Æ
	DeleteObject(hFont);
	DeleteObject(hFont2);
}