#pragma once
#include "Object.h"
class Player : public Object
{
	double speed = 10.;
public:
	Player(ObjectManager*, ObjectType objType, double);

    virtual void Update(double dt);
private:
	void KeyInput(double dt);
};