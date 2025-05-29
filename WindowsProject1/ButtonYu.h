#pragma once
#include "Object.h"

enum BUTTON_ID { BUTTON_FIRST, BUTTON_SECOND, BUTTON_THIRD, BUTTON_FOURTH, BUTTON_END };

class ButtonYu : public Object
{
public:
	BUTTON_ID m_eButtonID;
public:
	ButtonYu(ObjectManager* owner, ObjectType objType, BUTTON_ID eIDm, int* pC);

	virtual void Update(double dt);
	virtual void Render(HDC hdc) override;
private:
	void KeyInput(double dt);

	int* pCoin;
};

