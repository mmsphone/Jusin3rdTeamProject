#include "pch.h"
#include "Scene.h"
#include "Engine.h"
#include "Random.h"
#include "TransformComponent.h"
#include "Card.h"
#include "GameScene3.h"
#include "InputSystem.h"
#include "CardDeck.h"
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

	// 🔷 드로우 덱 오브젝트 생성
	m_pCardDeck = new CardDeck(objectManager.get());
	objectManager->AddObject(ObjectType::Back, std::shared_ptr<Object>(m_pCardDeck));

	auto pTransform = m_pCardDeck->GetComponent<TransformComponent>();
	pTransform->SetPosition(340.0f, 250.0f, 0.0f);
	pTransform->SetScale(50.0f, 70.0f, 0.0f);
}

	void GameScene3::Update(double dt) {
		if (m_eResult != eGameResult::None)
			return;
		objectManager->Update(dt);

		// 제출 카드 이동 완료 시 필드에 배치
		if (m_pSubmittedCard && !m_pSubmittedCard->IsMoving()) {
			m_pFieldCard = m_pSubmittedCard;
			m_pSubmittedCard = nullptr;
			CheckWinLoseCondition();
			NextTurn();
			m_bAITurnStarted = false;
		}
		// AI 턴 로직
		if (m_eTurn == eTurn::AI && !m_pSubmittedCard && !m_bAITurnStarted) {
			m_bAITurnStarted = true;

			// 1. 낼 수 있는 카드 탐색
			Card* pSelected = nullptr;
			for (Card* pCard : m_vecAIHand) {
				if (pCard->GetShape() == m_pFieldCard->GetShape() ||
					pCard->GetNumber() == m_pFieldCard->GetNumber()) {
					pSelected = pCard;
					break;
				}
			}

			if (pSelected) {
				// 낼 수 있는 카드 있음 → 제출
				pSelected->MoveToField(400.0f, 250.0f, 0.0f);
				m_pSubmittedCard = pSelected;

				m_vecAIHand.erase(
					std::remove(m_vecAIHand.begin(), m_vecAIHand.end(), pSelected),
					m_vecAIHand.end());
				ResetAIHandPosition();
			}
			else {
				// 낼 수 있는 카드 없음 → 드로우
				if (!m_vecDeck.empty()) {
					Card* pCard = m_vecDeck.back();
					m_vecDeck.pop_back();
					m_vecAIHand.push_back(pCard);
					ResetAIHandPosition();
					auto pT = pCard->GetComponent<TransformComponent>();
					pT->SetPosition(100.0f + (m_vecAIHand.size() - 1) * 60.0f, 100.0f, 0.0f);
					pT->SetScale(50.0f, 70.0f, 0.0f);
				}
				// 턴 넘김
				CheckWinLoseCondition();
				NextTurn();
				m_bAITurnStarted = false;
			}
		}
		// 제출 중이면 입력 무시
		if (m_eTurn != eTurn::Player || m_pSubmittedCard) return;

		auto input = Engine::GetInstance().GetInputSystem();
		if (input->IsMousePressed()) {
			D3DXVECTOR3 vMouse = input->GetMousePos();
			POINT pt = { (LONG)vMouse.x, (LONG)vMouse.y };

			// 1. 드로우 덱 클릭
			if (m_pCardDeck) {
				auto pTransform = m_pCardDeck->GetComponent<TransformComponent>();
				D3DXVECTOR3 vPos = pTransform->GetPosition();
				D3DXVECTOR3 vScale = pTransform->GetScale();
				RECT rc = {
					(int)(vPos.x - vScale.x * 0.5f),
					(int)(vPos.y - vScale.y * 0.5f),
					(int)(vPos.x + vScale.x * 0.5f),
					(int)(vPos.y + vScale.y * 0.5f)
				};
				if (PtInRect(&rc, pt)) {
					if (!m_vecDeck.empty()) {
						Card* pCard = m_vecDeck.back();
						m_vecDeck.pop_back();
						m_vecPlayerHand.push_back(pCard);
						ResetPlayerHandPosition();
						auto pT = pCard->GetComponent<TransformComponent>();
						pT->SetPosition(100.0f + (m_vecPlayerHand.size() - 1) * 60.0f, 400.0f, 0.0f);
						pT->SetScale(50.0f, 70.0f, 0.0f);
					}
					m_pSelectedCard = nullptr;
					CheckWinLoseCondition();
					NextTurn(); // 턴 전환
					return;
				}
			}

			// 2. 플레이어 카드 클릭
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

				if (PtInRect(&rc, pt)) {
					bool bCanSubmit =
						pCard->GetShape() == m_pFieldCard->GetShape() ||
						pCard->GetNumber() == m_pFieldCard->GetNumber();

					if (!bCanSubmit)
						break;

					if (m_pSelectedCard == pCard) {
						pCard->MoveToField(400.0f, 250.0f, 0.0f);
						m_pSubmittedCard = pCard;

						m_vecPlayerHand.erase(
							std::remove(m_vecPlayerHand.begin(), m_vecPlayerHand.end(), pCard),
							m_vecPlayerHand.end());
						ResetPlayerHandPosition();
						m_pSelectedCard = nullptr;
					}
					else {
						if (m_pSelectedCard) {
							auto prev = m_pSelectedCard->GetComponent<TransformComponent>();
							prev->SetPosition(prev->GetPosition().x, 400.0f, 0.0f);
						}

						auto sel = pCard->GetComponent<TransformComponent>();
						sel->SetPosition(sel->GetPosition().x, 370.0f, 0.0f);

						m_pSelectedCard = pCard;
					}
					break;
				}
			}
		}
	}

void GameScene3::Render(HDC hdc) {
	if (m_eResult == eGameResult::PlayerWin)
		TextOutA(hdc, 300, 50, "YOU WIN!", 8);
	else if (m_eResult == eGameResult::PlayerLose)
		TextOutA(hdc, 300, 50, "YOU LOSE!", 9);
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

void GameScene3::NextTurn() {
	if (m_eTurn == eTurn::Player) {
		m_eTurn = eTurn::AI;
	}
	else {
		m_eTurn = eTurn::Player;
	}
}

void GameScene3::ResetAIHandPosition() {
	for (int i = 0; i < (int)m_vecAIHand.size(); ++i) {
		auto pTransform = m_vecAIHand[i]->GetComponent<TransformComponent>();
		pTransform->SetPosition(100.0f + i * 60.0f, 100.0f, 0.0f);
		pTransform->SetScale(50.0f, 70.0f, 0.0f);
	}
}

void GameScene3::ResetPlayerHandPosition() {
	for (int i = 0; i < (int)m_vecPlayerHand.size(); ++i) {
		auto pTransform = m_vecPlayerHand[i]->GetComponent<TransformComponent>();
		pTransform->SetPosition(100.0f + i * 60.0f, 400.0f, 0.0f);
		pTransform->SetScale(50.0f, 70.0f, 0.0f);
	}
}

void GameScene3::CheckWinLoseCondition() {
	if (m_vecPlayerHand.size() == 0) {
		m_eResult = eGameResult::PlayerWin;
	}
	else if (m_vecPlayerHand.size() >= 20) {
		m_eResult = eGameResult::PlayerLose;
	}
	else if (m_vecAIHand.size() == 0) {
		m_eResult = eGameResult::PlayerLose;
	}
	else if (m_vecAIHand.size() >= 20) {
		m_eResult = eGameResult::PlayerWin;
	}
}

bool GameScene3::IsAttackCard(Card* pCard) {
	auto num = pCard->GetNumber();
	auto shape = pCard->GetShape();

	if (num == eCardNumber::N2 || num == eCardNumber::A)
		return true;
	return false;
}

int GameScene3::GetAttackDrawCount(Card* pCard) {
	auto num = pCard->GetNumber();
	auto shape = pCard->GetShape();

	if (num == eCardNumber::N2) return 2;
	if (num == eCardNumber::A && shape != eCardShape::S) return 3;
	if (num == eCardNumber::A && shape == eCardShape::S) return 5;
	return 0;
}

bool GameScene3::CanDefend(Card* pCard, Card* pAttackingCard) {
	if (!pAttackingCard) return false;

	auto atkNum = pAttackingCard->GetNumber();
	auto atkShape = pAttackingCard->GetShape();
	auto defNum = pCard->GetNumber();
	auto defShape = pCard->GetShape();

	if (atkNum == eCardNumber::N2) {
		if (defNum == eCardNumber::N3 && defShape == atkShape) return true;
		if (defNum == eCardNumber::A && defShape == atkShape) return true;
		if (defNum == eCardNumber::N2 && defShape != atkShape) return true;
	}
	else if (atkNum == eCardNumber::A && atkShape != eCardShape::S) {
		if (defNum == eCardNumber::A && defShape != atkShape) return true;
	}
	else if (atkNum == eCardNumber::A && atkShape == eCardShape::S) {
		return false; // 방어 불가
	}

	return false;
}