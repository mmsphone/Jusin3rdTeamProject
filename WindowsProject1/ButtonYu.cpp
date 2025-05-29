#include "pch.h"
#include "ButtonYu.h"
#include "TransformComponent.h"
#include "Engine.h"
#include "InputSystem.h"
#include "TextureManager.h"
#include "ObjectManager.h"
#include "CoinYu.h"





ButtonYu::ButtonYu(ObjectManager* owner, ObjectType objType, BUTTON_ID eID, int* pC)
	:Object(owner, objType), m_eButtonID(eID), pCoin(pC)
{
	auto transform = AddComponent<TransformComponent>();
	switch (eID) {
	case BUTTON_FIRST:
		transform->SetPosition(80, 100);
		break;
	case BUTTON_SECOND:
		transform->SetPosition(80, 200);
		break;
	case BUTTON_THIRD:
		transform->SetPosition(80, 300);
		break;
	case BUTTON_FOURTH:
		transform->SetPosition(80, 400);
		break;
	}
	transform->SetScale(190.0f, 95.0f);
	
	Engine::GetInstance().GetTextureManager()->InsertTexture(L"../Image/1times.bmp", L"1times", 190, 100);
	Engine::GetInstance().GetTextureManager()->InsertTexture(L"../Image/2times.bmp", L"2times", 190, 100);
	Engine::GetInstance().GetTextureManager()->InsertTexture(L"../Image/5times.bmp", L"5times", 190, 100);
	Engine::GetInstance().GetTextureManager()->InsertTexture(L"../Image/10times.bmp", L"10times", 190, 100);

}

void ButtonYu::Update(double dt)
{
	KeyInput(dt);
}

void ButtonYu::Render(HDC hdc)
{

	POINT pt{};
	std::wstring Key;
	switch (m_eButtonID) {
	case BUTTON_FIRST:
		Key = L"1times";
		pt = { -20, 100 };
		break;
	case BUTTON_SECOND:
		Key = L"2times";
		pt = { -20, 200 };
		break;
	case BUTTON_THIRD:
		Key = L"5times";
		pt = { -20, 300 };
		break;
	case BUTTON_FOURTH:
		Key = L"10times";
		pt = { -20, 400 };
		break;
	}

	HDC ButtonDC = Engine::GetInstance().GetTextureManager()->FindTexture(Key);

	GdiTransparentBlt(hdc, pt.x, pt.y, 190, 100, ButtonDC, 0, 0, 190, 100, RGB(255, 0, 255));


}

void ButtonYu::KeyInput(double dt)
{
	auto input = Engine::GetInstance().GetInputSystem();
	D3DXVECTOR3 MousePos = input->GetMousePos();

	if (input->IsMouseDown()) {
		if (MousePos.x > 0 && MousePos.x < 170) {
			if (*pCoin < 5) return;

			if (MousePos.y > 100 && MousePos.y < 200) {
				owner->AddObject(ObjectType::Mid, std::make_shared<CoinYu>(owner, ObjectType::Mid,50,150));
			}
			else if (MousePos.y > 200 && MousePos.y < 300) {
				owner->AddObject(ObjectType::Mid, std::make_shared<CoinYu>(owner, ObjectType::Mid, 50, 250));
			}
			else if (MousePos.y > 300 && MousePos.y < 400) {
				owner->AddObject(ObjectType::Mid, std::make_shared<CoinYu>(owner, ObjectType::Mid, 50, 350));
			}
			else if (MousePos.y > 400 && MousePos.y < 500) {
				owner->AddObject(ObjectType::Mid, std::make_shared<CoinYu>(owner, ObjectType::Mid, 50, 450));
			}
		}
	}
	
}
