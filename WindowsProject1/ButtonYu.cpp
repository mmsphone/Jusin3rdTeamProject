#include "pch.h"
#include "ButtonYu.h"
#include "TransformComponent.h"
#include "Engine.h"
#include "InputSystem.h"





ButtonYu::ButtonYu(ObjectManager* owner, ObjectType objType, BUTTON_ID eID)
	:Object(owner, objType), m_eButtonID(eID)
{
	auto transform = AddComponent<TransformComponent>();
	float StartPointX = 250.f;
	switch (eID) {
	case BUTTON_FIRST:
		transform->SetPosition(StartPointX, 500.f);
		break;
	case BUTTON_SECOND:
		transform->SetPosition(StartPointX + 100, 500.f);
		break;
	case BUTTON_THIRD:
		transform->SetPosition(StartPointX + 200, 500.f);
		break;
	case BUTTON_FOURTH:
		transform->SetPosition(StartPointX + 300, 500.f);
		break;
	}
	transform->SetScale(95.0f, 30.0f);
}

void ButtonYu::Update(double dt)
{
}

void ButtonYu::KeyInput(double dt)
{
	auto transform = GetComponent<TransformComponent>();
	if (!transform) return;

	auto input = Engine::GetInstance().GetInputSystem();
	if (input->IsKeyDown('A')) {
		if (BUTTON_FIRST == m_eButtonID) {
			
		}
	}
	if (input->IsKeyDown('B')) {
	}
	if (input->IsKeyDown(VK_SPACE)) {
	}
}
