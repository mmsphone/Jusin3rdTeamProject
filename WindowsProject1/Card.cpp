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

		if (fLen < 10.0f) {
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

std::string Card::GetCardCode() {
	std::string strCode;

	switch (eShape) {
	case eCardShape::H: strCode += "H"; break;
	case eCardShape::S: strCode += "S"; break;
	case eCardShape::C: strCode += "C"; break;
	case eCardShape::D: strCode += "D"; break;
	}

	int iNumber = static_cast<int>(eNumber);
	if (iNumber >= 2 && iNumber <= 10) {
		strCode += std::to_string(iNumber);
	}
	else {
		switch (eNumber) {
		case eCardNumber::A: strCode += "A"; break;
		case eCardNumber::J: strCode += "J"; break;
		case eCardNumber::Q: strCode += "Q"; break;
		case eCardNumber::K: strCode += "K"; break;
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

	auto pTransform = GetComponent<TransformComponent>();
	if (!pTransform) return; // TransformComponent가 없으면 출력하지 않음

	D3DXVECTOR3 vPos = pTransform->GetPosition();

	std::string strCode = GetCardCode();
	int iTextLen = static_cast<int>(strCode.length());

	TextOutA(hdc, static_cast<int>(vPos.x) - 10, static_cast<int>(vPos.y) - 10,
		strCode.c_str(), iTextLen);
}
