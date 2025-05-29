#include "pch.h"
#include "Card.h"
#include "TransformComponent.h"

Card::Card(ObjectManager* owner, eCardShape eShape, eCardNumber eNumber)
	: Object(owner,ObjectType::Mid), eShape(eShape), eNumber(eNumber) {
	AddComponent<TransformComponent>();
}

void Card::Update(double dt) {
	Object::Update(dt); // 기존 Update 유지
	if (m_bMoveToField) {
		auto pTransform = GetComponent<TransformComponent>();
		D3DXVECTOR3 vPos = pTransform->GetPosition();

		D3DXVECTOR3 vDir = m_vTargetPos - vPos;
		float fLen = D3DXVec3Length(&vDir);

		if (fLen < fSpeed*0.05) {
			pTransform->SetPosition(m_vTargetPos.x, m_vTargetPos.y, m_vTargetPos.z);
			m_bMoveToField = false;
		}
		else {
			D3DXVec3Normalize(&vDir, &vDir);
			vPos += vDir * fSpeed * dt;
			pTransform->SetPosition(vPos.x, vPos.y, vPos.z);
		}
	}
}

std::wstring Card::GetCardCode() {
	std::wstring strCode;

	switch (eShape) {
	case eCardShape::H: strCode += L"♥"; break;
	case eCardShape::S: strCode += L"♠"; break;
	case eCardShape::C: strCode += L"♣"; break;
	case eCardShape::D: strCode += L"♦"; break;
	}

	int iNumber = static_cast<int>(eNumber);
	if (iNumber >= 2 && iNumber <= 10) {
		strCode += std::to_wstring(iNumber);
	}
	else {
		switch (eNumber) {
		case eCardNumber::A: strCode += L"A"; break;
		case eCardNumber::J: strCode += L"J"; break;
		case eCardNumber::Q: strCode += L"Q"; break;
		case eCardNumber::K: strCode += L"K"; break;
		default: break;
		}
	}

	return strCode;
}

void Card::MoveToField(float x, float y, float z) {
	m_bMoveToField = true;
	m_vTargetPos = { x, y, z };
}

void Card::Render(HDC hdc) {
	Object::Render(hdc); // 사각형 출력 (점 4개)

	if (!m_bVisible)  return;

	auto pTransform = GetComponent<TransformComponent>();
	if (!pTransform) return;

	D3DXVECTOR3 vPos = pTransform->GetPosition();

	std::wstring strCode = GetCardCode();
	int iTextLen = static_cast<int>(strCode.length());

	// 🔷 글자 출력 전 배경 지우기
	RECT textRect = {
		(int)(vPos.x - 15),
		(int)(vPos.y - 30),
		(int)(vPos.x + 15),
		(int)(vPos.y + 30)
	};
	FillRect(hdc, &textRect, (HBRUSH)(COLOR_WINDOW + 1)); // 시스템 배경색

	// 🔷 텍스트 출력
	DrawTextW(hdc, strCode.c_str(), -1, &textRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}