#include "pch.h"
#include "BigWheel.h"
#include "TransformComponent.h"
#include "Engine.h"
#include "InputSystem.h"
#include "ObjectManager.h"
#include "TextureManager.h"




BigWheel::BigWheel(ObjectManager*, ObjectType obType)
	:Object(owner, objType, RenderType::Rect)
{
	auto transform = AddComponent<TransformComponent>();
	transform->SetPosition(400.0f, 300.0f);
	transform->SetScale(5.0f, 300.0f);

	CenterPt = { 400,300 };
	fCenterAngle = 0.f;
	CurAngle = 0;
	Engine::GetInstance().GetTextureManager()->InsertTexture(L"../Image/Roulette.bmp", L"Roulette", 500, 500);

	//#ifdef _DEBUG
	//
	//if (::AllocConsole() == TRUE)
	//{
	//	FILE* nfp[3];
	//	freopen_s(nfp + 0, "CONOUT$", "rb", stdin);
	//	freopen_s(nfp + 1, "CONOUT$", "wb", stdout);
	//	freopen_s(nfp + 2, "CONOUT$", "wb", stderr);
	//	std::ios::sync_with_stdio();
	//}
	//
	//#endif // _DEBUG
}

void BigWheel::Update(double dt)
{
	KeyInput(dt);
	if (bSpin) Spin(dt);
//#ifdef _DEBUG
//
//	std::cout << CurAngle << std::endl;
//
//#endif // _DEBUG
}

void BigWheel::Render(HDC hDC)
{
	//Object::Render(hDC);
	Make_Grid(hDC);

	auto transform = GetComponent<TransformComponent>();
	if (!transform) return; // 없으면 그리지 않음

	D3DXVECTOR3 pos = transform->GetPosition();
	D3DXVECTOR3 scale = transform->GetScale();
	float rotateZ = transform->GetRotationZ();

	float halfW = scale.x * 0.5f;
	float halfH = scale.y * 0.5f;
	int cx = static_cast<int>(pos.x);
	int cy = static_cast<int>(pos.y);

	if (type == RenderType::Rect) {
		// 꼭짓점 기준 좌표 (중심 기준)
		POINT points[4];

		float cosTheta = cosf(rotateZ);
		float sinTheta = sinf(rotateZ);

		D3DXVECTOR3 localCorners[4] = {
			{ -halfW, -halfH, 0 }, // LT
			{  halfW, -halfH, 0 }, // RT
			{  halfW,  -10, 0 }, // RB
			{ -halfW,  -10, 0 }  // LB
		};

		for (int i = 0; i < 4; ++i) {
			float x = localCorners[i].x;
			float y = localCorners[i].y;

			// 회전 적용
			float rx = x * cosTheta - y * sinTheta;
			float ry = x * sinTheta + y * cosTheta;

			points[i].x = static_cast<LONG>(cx + rx);
			points[i].y = static_cast<LONG>(cy + ry);
		}

		// 선으로 사각형 그리기
		MoveToEx(hDC, points[0].x, points[0].y, nullptr);
		for (int i = 1; i < 4; ++i) {
			LineTo(hDC, points[i].x, points[i].y);
		}
		LineTo(hDC, points[0].x, points[0].y);

		//MoveToEx(hDC, points[0].x, points[0].y, nullptr);
		//LineTo(hDC, points[0].x - 5, points[0].y);
		//LineTo(hDC, points[0].x + 5, points[0].y-20);
		//LineTo(hDC, points[1].x + 5, points[1].y);
		//LineTo(hDC, points[0].x, points[0].y);



	}
	Ellipse(hDC, 385, 285, 415, 315);
	
}

void BigWheel::KeyInput(double dt)
{
	auto transform = GetComponent<TransformComponent>();
	if (!transform) return;

	auto input = Engine::GetInstance().GetInputSystem();
	if (input->IsKeyDown(VK_LEFT)) {
		transform->RotateZ(dAngleSpeed);
	}
	if (input->IsKeyDown(VK_RIGHT)) {
		transform->RotateZ(-dAngleSpeed);
	}
	if (input->IsKeyDown(VK_SPACE)) {
		if (!bSpin) bSpin = true;
	}
}

void BigWheel::Spin(double dt)
{
	//CurVal = Lerp(CurVal, (double)0, float(dAngleSpeed * dt));
	elapsed += dt;
	float currentSpeed = initialSpeed * std::exp(-decayRate * elapsed);
	auto transform = GetComponent<TransformComponent>();
	transform->RotateZ(currentSpeed);
	D3DXMATRIX matOr, matTCenter;
	
	rotationAngle += currentSpeed * dt;



	CurAngle = int(transform->GetRotationZ()) % 360 ;
	
	if (currentSpeed < MIN_SPEED) {
		bSpin = false;
		elapsed = 0.0f;
	}



	
	
}

void BigWheel::Make_Grid(HDC hDC)
{
	RECT rc{ 150, 50, 650, 550 };

	HDC RouletteDC = Engine::GetInstance().GetTextureManager()->FindTexture(L"Roulette");
	
	GdiTransparentBlt(hDC, rc.left, rc.top, 500, 500, RouletteDC, 0, 0, 500, 500, RGB(255, 0, 255));
	
	//Ellipse(hDC, 150, 50, 650, 550);
	
	//int iGridNum = 10;
	//int Radius = 250;
	//double fGridAngle = 2 * PI / iGridNum;
	//for (int i = 0; i < iGridNum; ++i)
	//{
	//	double theta = fGridAngle * i;
	//	int x = CenterPt.x + static_cast<int>(cos(theta) * Radius);
	//	int y = CenterPt.y - static_cast<int>(sin(theta) * Radius);
	//
	//	MoveToEx(hDC, x, y, nullptr);
	//	LineTo(hDC, CenterPt.x, CenterPt.y);
	//
	//}


}
