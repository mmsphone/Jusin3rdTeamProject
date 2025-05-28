#pragma once
#include "Object.h"
#include "ButtonYu.h"

//enum BUTTON_ID : int;

class NoteYu : public Object
{
public:
	BUTTON_ID m_eButtonIDl;
	double m_iSpeed;

public:
	NoteYu(ObjectManager* owner, ObjectType objType, BUTTON_ID eID, double speed);
	

	virtual void Update(double dt);


};

