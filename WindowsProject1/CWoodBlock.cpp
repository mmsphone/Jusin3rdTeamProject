#include "pch.h"
#include "CWoodBlock.h"
#include "TransformComponent.h"
#include "CollisionComponent.h"
#include "Engine.h"
#include "InputSystem.h"
#include "ObjectManager.h"


CWoodBlock::CWoodBlock(ObjectManager* owner, ObjectType objType, double speed)
	: Object(owner, objType, RenderType::Rect), speed(speed)
{
	auto transform = AddComponent<TransformComponent>();
	transform->SetPosition(0.0f, 0.0f);
	transform->SetScale(40.0f, 40.0f);

	auto collision = AddComponent<CollisionComponent>();
	collision->Initialize();

}
void CWoodBlock::Update(double dt)
{
	KeyInput(dt);
}
void CWoodBlock::KeyInput(double dt)
{
	// auto transform = GetComponent<TransformComponent>();
	// if (!transform) return;
	// 
	// auto input = Engine::GetInstance().GetInputSystem();
	// if (input->IsKeyDown('E')) {
	// 	transform->SetRotationZ(transform->GetRotationZ() + D3DXToRadian(3.f));
	// }
}


void CWoodBlock::Render(HDC hdc) {

	auto transform = GetComponent<TransformComponent>();
	if (!transform) return; // 없으면 그리지 않음

	COLORREF colorSet[4] = {
	RGB(208, 182, 154),  // left
	RGB(191, 157, 119), // top
	RGB(106, 80, 52),   // right
	RGB(141, 107, 69)  // bottom
	};

	HPEN linePen2 = CreatePen(PS_SOLID, 2, RGB(106, 80, 52));
	HPEN oldPen;
	HBRUSH backBrush = CreateSolidBrush(RGB(191, 157, 119));

	int iRotationStep = ((int)(D3DXToDegree(transform->GetRotationZ()) + 45) % 360) / 90;

	HBRUSH leftBlockBrush = CreateSolidBrush(colorSet[(0 + iRotationStep) % 4]);
	HBRUSH topBlockBrush = CreateSolidBrush(colorSet[(1 + iRotationStep) % 4]);
	HBRUSH rightBlockBrush = CreateSolidBrush(colorSet[(2 + iRotationStep) % 4]);
	HBRUSH bottomBlockBrush = CreateSolidBrush(colorSet[(3 + iRotationStep) % 4]);

	HBRUSH oldBrush;



	D3DXVECTOR3 pos = transform->GetPosition();
	D3DXVECTOR3 scale = transform->GetScale();
	float rotateZ = transform->GetRotationZ();

	float halfW = scale.x * 0.5f;
	float halfH = scale.y * 0.5f;
	int cx = static_cast<int>(pos.x);
	int cy = static_cast<int>(pos.y);

	float cosTheta = cosf(rotateZ);
	float sinTheta = sinf(rotateZ);

	auto RotatePoint = [&](float x, float y, float cx, float cy, float cosTheta, float sinTheta) -> POINT {
		float dx = x - cx;
		float dy = y - cy;

		float rx = dx * cosTheta - dy * sinTheta;
		float ry = dx * sinTheta + dy * cosTheta;

		return {
			static_cast<LONG>(cx + rx),
			static_cast<LONG>(cy + ry)
		};
		};

		//바깥쪽
		float Block_left = cx - halfW;
		float Block_right = cx + halfW;
		float Block_top = cy - halfH;
		float Block_bottom = cy + halfH;

		// 안쪽
		float Block_inset = 5.f;
		float Block_inleft = Block_left + Block_inset;
		float Block_inright = Block_right - Block_inset;
		float Block_intop = Block_top + Block_inset;
		float Block_inbottom = Block_bottom - Block_inset;

		POINT outer[4] = {
			RotatePoint(Block_left, Block_top, cx, cy, cosTheta, sinTheta),
			RotatePoint(Block_right, Block_top, cx, cy, cosTheta, sinTheta),
			RotatePoint(Block_right, Block_bottom, cx, cy, cosTheta, sinTheta),
			RotatePoint(Block_left, Block_bottom, cx, cy, cosTheta, sinTheta)
		};

		POINT inner[4] = {
			RotatePoint(Block_inleft, Block_intop, cx, cy, cosTheta, sinTheta),
			RotatePoint(Block_inright, Block_intop, cx, cy, cosTheta, sinTheta),
			RotatePoint(Block_inright, Block_inbottom, cx, cy, cosTheta, sinTheta),
			RotatePoint(Block_inleft, Block_inbottom, cx, cy, cosTheta, sinTheta)
		};

		POINT Block_leftPoints[4] = { outer[0], outer[3], inner[3], inner[0] };
		POINT Block_rightPoint[4] = { outer[1], outer[2], inner[2], inner[1] };
		POINT Block_topPoint[4] = { outer[0], outer[1], inner[1], inner[0] };
		POINT Block_bottomPoint[4] = { outer[2], outer[3], inner[3], inner[2] };


		oldPen = (HPEN)SelectObject(hdc, linePen2);


		oldBrush = (HBRUSH)SelectObject(hdc, leftBlockBrush);
		Polygon(hdc, Block_leftPoints, 4);
		SelectObject(hdc, oldBrush);

		oldBrush = (HBRUSH)SelectObject(hdc, rightBlockBrush);
		Polygon(hdc, Block_rightPoint, 4);
		SelectObject(hdc, oldBrush);

		oldBrush = (HBRUSH)SelectObject(hdc, topBlockBrush);
		Polygon(hdc, Block_topPoint, 4);
		SelectObject(hdc, oldBrush);

		oldBrush = (HBRUSH)SelectObject(hdc, bottomBlockBrush);
		Polygon(hdc, Block_bottomPoint, 4);
		SelectObject(hdc, oldBrush);

		oldBrush = (HBRUSH)SelectObject(hdc, backBrush);
		Polygon(hdc, inner, 4);
		SelectObject(hdc, oldBrush);


		SelectObject(hdc, oldPen);
	


	DeleteObject(backBrush);
	DeleteObject(leftBlockBrush);
	DeleteObject(rightBlockBrush);
	DeleteObject(topBlockBrush);
	DeleteObject(bottomBlockBrush);
	DeleteObject(linePen2);
}