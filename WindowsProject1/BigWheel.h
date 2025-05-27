#pragma once
#include "Object.h"

template<typename T>
T Lerp(T a, T b, float t) {
	return a + (b - a) * t;
}

class BigWheel : public Object
{
public:
	BigWheel(ObjectManager*, ObjectType obType);

	virtual void Update(double dt);
	virtual void Render(HDC hDC) override;
	



private:
	void KeyInput(double dt);
	void Spin(double dt);
	void Make_Grid();


	double timer = 0;
	bool	bSpin = false;


private:
	double CurVal = 100;
	double dAngleSpeed = 5.f;


	



};

