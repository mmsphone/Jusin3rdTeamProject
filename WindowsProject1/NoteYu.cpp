#include "pch.h"
#include "NoteYu.h"
#include "TransformComponent.h"

NoteYu::NoteYu(ObjectManager* owner, ObjectType objType, BUTTON_ID eID, double speed)
	:Object(owner, objType), m_iSpeed(speed)
{
	m_eButtonIDl = eID;

	auto transform = AddComponent<TransformComponent>();

	float fX(0);
	float StartPointX = 250.f;
	switch (eID) {
	case BUTTON_FIRST:
		fX = StartPointX;
		break;
	case BUTTON_SECOND:
		fX = StartPointX + 100;
		break;
	case BUTTON_THIRD:
		fX = StartPointX + 200;
		break;
	case BUTTON_FOURTH:
		fX = StartPointX + 300;
		break;
	}

	transform->SetPosition(fX, 0.f);
	transform->SetScale(95.f, 40.f);
}

void NoteYu::Update(double dt)
{
	auto transform = GetComponent<TransformComponent>();
	transform->Translate(0, m_iSpeed * static_cast<float>(dt));


}
