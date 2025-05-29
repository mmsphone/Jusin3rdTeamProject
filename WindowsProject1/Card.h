#pragma once
#include "Object.h"

enum class eCardShape {
	H, // 하트
	S, // 스페이드
	C, // 클로버
	D  // 다이아
};

enum class eCardNumber {
	A = 1,
	N2, N3, N4, N5, N6, N7, N8, N9, N10,
	J, Q, K
};

class Card : public Object {
private:
	eCardShape eShape;
	eCardNumber eNumber;
	bool m_bMoveToField = false;
	D3DXVECTOR3 m_vTargetPos;
	float fSpeed = 800.f;
	bool m_bVisible=false;
public:
	Card(ObjectManager* owner, eCardShape eShape, eCardNumber eNumber);
	void Update(double dt) override;
	void Render(HDC hdc) override;
	std::wstring GetCardCode();
	eCardShape GetShape() const { return eShape; }
	eCardNumber GetNumber() const { return eNumber; }
	bool IsMoving() const { return m_bMoveToField; };
	void MoveToField(float x, float y, float z = 0.0f);
	void SetVisible(bool _visible) { m_bVisible = _visible; }
};
