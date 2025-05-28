#pragma once
#include "Object.h"

enum BUTTON_ID { BUTTON_FIRST, BUTTON_SECOND, BUTTON_THIRD, BUTTON_FOURTH, BUTTON_END };

class ButtonYu : public Object
{
public:
	BUTTON_ID m_eButtonID;
public:
	ButtonYu(ObjectManager* owner, ObjectType objType, BUTTON_ID eID);

	virtual void Update(double dt);

private:
	void KeyInput(double dt);
};

