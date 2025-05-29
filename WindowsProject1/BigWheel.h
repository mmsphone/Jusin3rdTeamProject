#pragma once
#include "Object.h"
#include <string>

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
	void Make_Grid(HDC hDC);

	double timer = 0;
	bool	bSpin = false;
	TCHAR	m_szTypeName[32];

private:
	double CurVal = 10;
	double dAngleSpeed = 1.f;

	float rotationAngle = 0.0f;     // ���� ȸ�� ����
	float elapsed = 0.0f;			// ���� �ð� (��)

	float initialSpeed = 320.0f;    // ���� ȸ�� �ӵ� (��/��)
	float decayRate = 1.5f;			// ���� ��� (Ŭ���� ���� ������)
	float MIN_SPEED = 0.000000001f; // �� �ӵ� ���ϰ� �Ǹ� ����
	
	double CurAngle;

private:
	POINT CenterPt;
	float fCenterAngle;
	
};

