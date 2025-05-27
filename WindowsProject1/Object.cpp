#include "pch.h"
#include "Object.h"
#include "TransformComponent.h"

Object::Object(ObjectManager* owner, ObjectType objType, RenderType type) : owner(owner), isDead(false), type(type), currState(&stateNothing) {}

void Object::Update(double dt)
{
	
	for (auto& component : components)
	{
		component->Update(dt);
	}	
	currState->Update(this, dt);
	currState->TestForExit(this);
}

void Object::Render(HDC hdc) 
{
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
			{  halfW,  halfH, 0 }, // RB
			{ -halfW,  halfH, 0 }  // LB
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
		MoveToEx(hdc, points[0].x, points[0].y, nullptr);
		for (int i = 1; i < 4; ++i) {
			LineTo(hdc, points[i].x, points[i].y);
		}
		LineTo(hdc, points[0].x, points[0].y); // 닫기
	}
	else if (type == RenderType::Ellipse) {
		Ellipse(hdc, cx - halfW, cy - halfH, cx + halfW, cy + halfH);
	}
}

void Object::ChangeState(State* newState)
{
	currState = newState;
	currState->Enter(this);
}
