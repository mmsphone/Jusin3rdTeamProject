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

	int halfWidth = static_cast<int>(scale.x * 0.5f);
	int halfHeight = static_cast<int>(scale.y * 0.5f);
	int x = static_cast<int>(pos.x);
	int y = static_cast<int>(pos.y);

	switch (type)
	{
	case RenderType::Rect:
		Rectangle(hdc, x - halfWidth, y - halfHeight, x + halfWidth, y + halfHeight);
		break;

	case RenderType::Ellipse:
		Ellipse(hdc, x - halfWidth, y - halfHeight, x + halfWidth, y + halfHeight);
		break;
	}
}

void Object::ChangeState(State* newState)
{
	currState = newState;
	currState->Enter(this);
}
