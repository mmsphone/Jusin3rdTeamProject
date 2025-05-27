#include "pch.h"
#include "BigWheel.h"
#include "TransformComponent.h"
#include "Engine.h"
#include "InputSystem.h"
#include "ObjectManager.h"




BigWheel::BigWheel(ObjectManager*, ObjectType obType)
	:Object(owner, objType, RenderType::Rect)
{
	auto transform = AddComponent<TransformComponent>();
	transform->SetPosition(400.0f, 200.0f);
	transform->SetScale(20.0f, 200.0f);


}

void BigWheel::Update(double dt)
{
	KeyInput(dt);
	Make_Grid();
	if (bSpin) Spin(dt);
}

void BigWheel::Render(HDC hDC)
{
	Object::Render(hDC);

	//auto transform = GetComponent<TransformComponent>();
	//D3DXVECTOR3 pos = transform->GetPosition();
	
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


	rotationAngle += currentSpeed * dt;

	if (currentSpeed < MIN_SPEED) {
		bSpin = false;
		elapsed = 0.0f;
	}



	
	
}

void BigWheel::Make_Grid()
{
	auto transform = GetComponent<TransformComponent>();
	D3DXVECTOR3 Pos = transform->GetPosition();
	D3DXVECTOR3 Scale = transform->GetScale();
	
	float halfW = Scale.x * 0.5f;
	float halfH = Scale.y * 0.5f;

	int cx = static_cast<int>(Pos.x);
	int cy = static_cast<int>(Pos.y);


}
