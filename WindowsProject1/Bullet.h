#pragma once
#include "Object.h"
class ObjectManager;
class Bullet : public Object
{
	double speed = 10.;
public:
	Bullet(ObjectManager*, ObjectType objType, double);
	virtual void Render(HDC hdc);
	virtual void Update(double dt);
};