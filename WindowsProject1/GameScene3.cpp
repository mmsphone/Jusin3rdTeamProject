#include "pch.h"
#include "Scene.h"
#include "Engine.h"
#include "Random.h"
#include "TransformComponent.h"
#include "Card.h"
#include "GameScene3.h"
#include "InputSystem.h"

void GameScene3::Load() {
	Scene::Load();

	// 1. 덱 생성
	for (int iShape = 0; iShape < 4; ++iShape) {
		for (int iNum = 1; iNum <= 13; ++iNum) {
			eCardShape eShape = static_cast<eCardShape>(iShape);
			eCardNumber eNumber = static_cast<eCardNumber>(iNum);

			auto pCard = std::make_shared<Card>(objectManager.get(), eShape, eNumber);
			objectManager->AddObject(ObjectType::Mid,pCard);
			m_vecDeck.push_back(pCard.get());
		}
	}

	// ✅ 2. 덱 셔플 (Random을 통해 여러 번 swap 수행)
	Random& r = Random::GetInstance();
	for (int i = 0; i < (int)m_vecDeck.size(); ++i) {
		int iOther = r.GetRandom(0, (int)m_vecDeck.size() - 1);
		std::swap(m_vecDeck[i], m_vecDeck[iOther]);
	}

	// 3. 카드 분배: 플레이어, AI 각각 7장
	for (int i = 0; i < 7; ++i) {
		m_vecPlayerHand.push_back(m_vecDeck.back());
		m_vecDeck.pop_back();

		m_vecAIHand.push_back(m_vecDeck.back());
		m_vecDeck.pop_back();
	}

	// 4. 필드 카드 한 장 세팅
	m_pFieldCard = m_vecDeck.back();
	m_vecDeck.pop_back();

	// 5. 위치 배정
	for (int i = 0; i < (int)m_vecPlayerHand.size(); ++i) {
		auto pTransform = m_vecPlayerHand[i]->GetComponent<TransformComponent>();
		pTransform->SetPosition(100.0f + i * 60.0f, 400.0f, 0.0f);
		pTransform->SetScale(50.0f, 70.0f, 0.0f);
	}

	for (int i = 0; i < (int)m_vecAIHand.size(); ++i) {
		auto pTransform = m_vecAIHand[i]->GetComponent<TransformComponent>();
		pTransform->SetPosition(100.0f + i * 60.0f, 100.0f, 0.0f);
		pTransform->SetScale(50.0f, 70.0f, 0.0f);
	}

	if (m_pFieldCard) {
		auto pTransform = m_pFieldCard->GetComponent<TransformComponent>();
		pTransform->SetPosition(400.0f, 250.0f, 0.0f);
		pTransform->SetScale(50.0f, 70.0f, 0.0f);
	}
}

void GameScene3::Update(double dt) {
	// 오브젝트 전체 업데이트 (Card 포함됨)
	objectManager->Update(dt);

	// 제출 중인 카드가 이동 완료됐는지 체크
	if (m_pSubmittedCard && !m_pSubmittedCard->IsMoving()) {
		m_pFieldCard = m_pSubmittedCard;
		m_pSubmittedCard = nullptr;

		// TODO: AI 턴 전환
	}

	// 제출 중이면 입력 무시
	if (m_pSubmittedCard) return;

	auto input = Engine::GetInstance().GetInputSystem();
	if (input->IsMousePressed()) {
		D3DXVECTOR3 vMouse = input->GetMousePos();

		for (auto& pCard : m_vecPlayerHand) {
			auto pTransform = pCard->GetComponent<TransformComponent>();
			D3DXVECTOR3 vPos = pTransform->GetPosition();
			D3DXVECTOR3 vScale = pTransform->GetScale();

			RECT rc = {
				(int)(vPos.x - vScale.x * 0.5f),
				(int)(vPos.y - vScale.y * 0.5f),
				(int)(vPos.x + vScale.x * 0.5f),
				(int)(vPos.y + vScale.y * 0.5f)
			};

			if (PtInRect(&rc, { (LONG)vMouse.x, (LONG)vMouse.y })) {
				bool bCanSubmit =
					pCard->GetShape() == m_pFieldCard->GetShape() ||
					pCard->GetNumber() == m_pFieldCard->GetNumber();

				if (!bCanSubmit)
					break; // 제출 불가 카드 클릭 무시

				if (m_pSelectedCard == pCard) {
					// 제출 시작 (애니메이션 이동)
					pCard->MoveToField(400.0f, 250.0f, 0.0f);
					m_pSubmittedCard = pCard;

					// 핸드에서 제거, ObjectManager에는 그대로 유지됨
					m_vecPlayerHand.erase(
						std::remove(m_vecPlayerHand.begin(), m_vecPlayerHand.end(), pCard),
						m_vecPlayerHand.end());

					m_pSelectedCard = nullptr;

				}
				else {
					// 기존 선택 카드 원위치
					if (m_pSelectedCard) {
						auto prevTransform = m_pSelectedCard->GetComponent<TransformComponent>();
						prevTransform->SetPosition(prevTransform->GetPosition().x, 400.0f, 0.0f);
					}

					// 새 카드 선택 (살짝 위로)
					auto selTransform = pCard->GetComponent<TransformComponent>();
					selTransform->SetPosition(selTransform->GetPosition().x, 370.0f, 0.0f);

					m_pSelectedCard = pCard;
				}

				break;
			}
		}
	}
}

void GameScene3::Render(HDC hdc) {
	objectManager->Render(hdc);

	// 플레이어 핸드
	for (Card* pCard : m_vecPlayerHand) {
		pCard->Render(hdc);
	}

	// AI 핸드 (뒤집힌 카드로 대체 예정)
	for (Card* pCard : m_vecAIHand) {
		pCard->Render(hdc);
	}

	// 필드 카드
	if (m_pFieldCard)
		m_pFieldCard->Render(hdc);

	if (m_pSubmittedCard)
		m_pSubmittedCard->Render(hdc);
}

void GameScene3::Unload()
{
}
