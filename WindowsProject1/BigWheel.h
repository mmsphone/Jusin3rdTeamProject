#pragma once
#include "Object.h"

//template<typename T>
//T Lerp(T a, T b, float t) {
//	return a + (b - a) * t;
//}




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
	double CurVal = 10;
	double dAngleSpeed = 1.f;

	float rotationAngle = 0.0f;      // 누적 회전 각도
	float elapsed = 0.0f;      // 누적 시간 (초)

	float initialSpeed = 320.0f;    // 시작 회전 속도 (도/초)
	float decayRate = 1.5f;      // 감쇠 계수 (클수록 빨리 느려짐)
	float MIN_SPEED = 0.000000001f;  // 이 속도 이하가 되면 멈춤


};

