#include "pch.h"
#include "Scene.h"
#include "Engine.h"
#include "Random.h"
#include "TransformComponent.h"
#include "Card.h"
#include "GameScene3.h"
#include "InputSystem.h"
#include "CardDeck.h"
#include "ButtonObject.h"
#include "SceneManager.h"
#include "ShuffleEffect.h"
void GameScene3::Load() {
	Scene::Load();
	objectManager->Initialize();
	MakeButtons();
	m_vecDrawingCards.clear();
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

	ShuffleDeck();

	// 3. 카드 분배: 플레이어, AI 각각 7장
	for (int i = 0; i < 7; ++i) {
		Card* pPlayerCard = m_vecDeck.back();
		pPlayerCard->SetVisible(true); // 먼저 visible 설정
		m_vecPlayerHand.push_back(pPlayerCard);
		m_vecDeck.pop_back();
	}
	for (int i = 0; i < 7; ++i) {
		Card* pAICard = m_vecDeck.back();
		pAICard->SetVisible(false); // AI는 뒷면
		m_vecAIHand.push_back(pAICard);
		m_vecDeck.pop_back();
	}

	// 4. 필드 카드 한 장 세팅
	m_pFieldCard = m_vecDeck.back();
	m_pFieldCard->SetVisible(true);
	m_vecDeck.pop_back();

	// 5. 위치 배정
	for (int i = 0; i < (int)m_vecPlayerHand.size(); ++i) {
		auto pTransform = m_vecPlayerHand[i]->GetComponent<TransformComponent>();
		pTransform->SetPosition(50.0f + i * 60.0f, 550.0f, 0.0f);
		pTransform->SetScale(50.0f, 70.0f, 0.0f);
	}

	for (int i = 0; i < (int)m_vecAIHand.size(); ++i) {
		auto pTransform = m_vecAIHand[i]->GetComponent<TransformComponent>();
		pTransform->SetPosition(50.0f + i * 60.0f, 50.0f, 0.0f);
		pTransform->SetScale(50.0f, 70.0f, 0.0f);
	}

	if (m_pFieldCard) {
		auto pTransform = m_pFieldCard->GetComponent<TransformComponent>();
		pTransform->SetPosition(460.0f, 300.0f, 0.0f); // 중앙 오른쪽
		pTransform->SetScale(50.0f, 70.0f, 0.0f);
	}

	// 🔷 드로우 덱 오브젝트 생성
	m_pCardDeck = new CardDeck(objectManager.get());
	objectManager->AddObject(ObjectType::Back, std::shared_ptr<Object>(m_pCardDeck));

	auto pDeckTransform = m_pCardDeck->GetComponent<TransformComponent>();
	pDeckTransform->SetPosition(340.0f, 300.0f, 0.0f); // 중앙 왼쪽
	pDeckTransform->SetScale(50.0f, 70.0f, 0.0f);

	m_vecDeck.clear();
	RefillDeckIfEmpty();
}
void GameScene3::Update(double dt) {
	objectManager->Update(dt);

	if (m_bGameover) {
		m_fGameoverElapsed += dt;
		if (m_fGameoverElapsed >= 3.0) {
			Engine::GetInstance().sceneManager->SetActiveScene("Menu");
			return;
		}
	}
	if (m_bShuffling) {
		m_fShuffleElapsed += dt;
		if (m_fShuffleElapsed >= 5.0) {
			m_bShuffling = false;
		}
		return; // 셔플 중엔 아무것도 안 함
	}
	if (UpdateDrawingCard()) return;
	if (UpdateSubmittedCard()) return;
	if (m_bChoosingShape) return;
	if (m_eResult != eGameResult::None) return;
	if (m_bTurnDelayActive) {
		m_fTurnDelayElapsed += dt;
		if (m_fTurnDelayElapsed < 0.5) return;

		// 0.5초 경과 시 턴 처리 재개
		m_bTurnDelayActive = false;
	}

	switch (m_eTurn) {
	case eTurn::AI:
		UpdateAITurn();
		break;
	case eTurn::Player:
		UpdatePlayerInput();
		break;
	}
}
void GameScene3::Render(HDC hdc) {
	if (m_eResult == eGameResult::PlayerWin || m_eResult == eGameResult::PlayerLose) {
		const wchar_t* pszMessage = (m_eResult == eGameResult::PlayerWin) ? L"YOU WIN!" : L"YOU LOSE!";

		HFONT hFont = CreateFontW(
			72, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
			HANGUL_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
			DEFAULT_PITCH | FF_DONTCARE, L"맑은 고딕"
		);
		HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);

		RECT rcText = { 0, 200, 800, 500 }; // 중앙 상단 영역 (가로 폭 800 기준)
		DrawTextW(hdc, pszMessage, -1, &rcText, DT_CENTER | DT_TOP | DT_SINGLELINE);

		SelectObject(hdc, hOldFont);
		DeleteObject(hFont);
		return;
	}

	objectManager->Render(hdc);

	for (Card* pCard : m_vecPlayerHand) pCard->Render(hdc);
	for (Card* pCard : m_vecAIHand) pCard->Render(hdc); // 나중에 뒷면 처리 예정

	if (m_pFieldCard) {
		m_pFieldCard->Render(hdc);

		if (m_pFieldCard->GetNumber() == eCardNumber::N7 && m_bForceShapeActive) {
			std::wstring shapeText;

			switch (m_eForcedShape) {
			case eCardShape::S: shapeText = L"♠ Spade"; break;
			case eCardShape::H: shapeText = L"♥ Heart"; break;
			case eCardShape::D: shapeText = L"♦ Diamond"; break;
			case eCardShape::C: shapeText = L"♣ Club"; break;
			}

			TextOutW(hdc, 440, 240, shapeText.c_str(), (int)shapeText.length());
		}
		if (m_bAttackInProgress && m_pAttackingCard != nullptr) {
			std::wstring str = L"Draw x" + std::to_wstring(m_iAttackDrawCount);
			TextOutW(hdc, 440, 240, str.c_str(), (int)str.length());
		}
	}
	if (m_pSubmittedCard) m_pSubmittedCard->Render(hdc);

	if (m_bTurnDelayActive) {
		HFONT hFont = CreateFontW(32, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
			DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Arial");
		HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);

		RECT rc = { 0, 160, 800, 200 }; // x 중심이 400이고 세로 높이는 약간 여유
		DrawTextW(hdc, m_strTurnText.c_str(), -1, &rc, DT_CENTER | DT_TOP | DT_SINGLELINE);

		SelectObject(hdc, hOldFont);
		DeleteObject(hFont);
	}
}
void GameScene3::Unload() {
	// 드로우 카드 초기화
	m_vecDrawingCards.clear();

	// 카드덱 해제 (CardDeck은 shared_ptr로 등록했으니 여기선 따로 delete 필요 없음)
	m_pCardDeck = nullptr;

	// 필드 관련 카드
	m_pFieldCard = nullptr;
	m_pSelectedCard = nullptr;
	m_pSubmittedCard = nullptr;
	m_pAttackingCard = nullptr;

	// 카드 컨테이너 초기화
	m_vecDeck.clear();
	m_vecPlayerHand.clear();
	m_vecAIHand.clear();

	// 버튼 관련 해제
	for (ButtonObject* pBtn : m_vecShapeButtons) {
		auto sptr = std::shared_ptr<Object>(pBtn);  // objectManager에 등록된 shared_ptr 추정
		objectManager->RemoveObject(ObjectType::UI, sptr);
	}
	m_vecShapeButtons.clear();

	// 상태 초기화
	m_eTurn = eTurn::Player;
	m_eResult = eGameResult::None;
	m_bAITurnActive = false;
	m_bAttackInProgress = false;
	m_iAttackDrawCount = 0;
	m_bForceShapeActive = false;
	m_bChoosingShape = false;
	m_bGameover = false;
	m_fGameoverElapsed = 0.0;

	// 오브젝트 매니저에서 등록된 모든 오브젝트 제거
	objectManager->ClearObjectList(ObjectType::Back);
	objectManager->ClearObjectList(ObjectType::Mid);
	objectManager->ClearObjectList(ObjectType::UI);
}

void GameScene3::ShuffleDeck() {
	Random& rand = Random::GetInstance();
	for (int i = 0; i < (int)m_vecDeck.size(); ++i) {
		int iOther = rand.GetRandom(0, (int)m_vecDeck.size() - 1);
		std::swap(m_vecDeck[i], m_vecDeck[iOther]);
	}
}
void GameScene3::RefillDeckIfEmpty() {
	if (!m_vecDeck.empty()) return;

	std::vector<Card*> vecUsedCards;

	for (auto& pObj : objectManager->GetObjectList(ObjectType::Mid)) {
		Card* pCard = dynamic_cast<Card*>(pObj.get());
		if (!pCard) continue;

		if (pCard == m_pFieldCard) continue;
		if (std::find(m_vecPlayerHand.begin(), m_vecPlayerHand.end(), pCard) != m_vecPlayerHand.end()) continue;
		if (std::find(m_vecAIHand.begin(), m_vecAIHand.end(), pCard) != m_vecAIHand.end()) continue;

		vecUsedCards.push_back(pCard);
	}

	for (Card* pCard : vecUsedCards) {
		m_vecDeck.push_back(pCard);
		pCard->SetVisible(true); // 보이도록 설정
	}

	ShuffleDeck();

	auto pEffect = new ShuffleEffect(objectManager.get(), vecUsedCards.size(),
		m_pFieldCard->GetComponent<TransformComponent>()->GetPosition(),
		m_pCardDeck->GetComponent<TransformComponent>()->GetPosition());

	objectManager->AddObject(ObjectType::Effect, std::shared_ptr<Object>(pEffect));

	// 🔷 셔플 중 상태로 설정
	m_bShuffling = true;
	m_fShuffleElapsed = 0.0;
}
void GameScene3::NextTurn() {
	m_eTurn = (m_eTurn == eTurn::Player) ? eTurn::AI : eTurn::Player;
	m_bTurnDelayActive = true;
	m_fTurnDelayElapsed = 0.0;
	m_strTurnText = (m_eTurn == eTurn::Player) ? L"Your Turn" : L"AI Turn";
}

void GameScene3::ResetAIHandPosition() {
	for (int i = 0; i < (int)m_vecAIHand.size(); ++i) {
		auto pTransform = m_vecAIHand[i]->GetComponent<TransformComponent>();
		pTransform->SetPosition(50.0f + i * 60.0f, 50.0f, 0.0f);
		pTransform->SetScale(50.0f, 70.0f, 0.0f);
	}
}
void GameScene3::ResetPlayerHandPosition() {
	for (int i = 0; i < (int)m_vecPlayerHand.size(); ++i) {
		auto pTransform = m_vecPlayerHand[i]->GetComponent<TransformComponent>();
		pTransform->SetPosition(50.0f + i * 60.0f, 550.0f, 0.0f);
		pTransform->SetScale(50.0f, 70.0f, 0.0f);
	}
}

void GameScene3::CheckWinLoseCondition() {
	size_t iPlayerCount = m_vecPlayerHand.size();
	size_t iAICount = m_vecAIHand.size();

	if (iPlayerCount == 0) {
		m_eResult = eGameResult::PlayerWin;
		m_bGameover = true;
	}
	else if (iPlayerCount >= 13) {
		m_eResult = eGameResult::PlayerLose;
		m_bGameover = true;
	}
	else if (iAICount == 0) {
		m_eResult = eGameResult::PlayerLose;
		m_bGameover = true;
	}
	else if (iAICount >= 13) {
		m_eResult = eGameResult::PlayerWin;
		m_bGameover = true;
	}
}

bool GameScene3::IsAttackCard(Card* pCard) {
	eCardNumber eNum = pCard->GetNumber();
	return (eNum == eCardNumber::N2 || eNum == eCardNumber::A);
}
bool GameScene3::IsFunctionCard(Card* pCard) {
	eCardNumber eNum = pCard->GetNumber();
	return (eNum == eCardNumber::J || eNum == eCardNumber::Q || eNum == eCardNumber::K);
}
int GameScene3::GetAttackDrawCount(Card* pCard) {
	eCardNumber eNum = pCard->GetNumber();
	eCardShape eShape = pCard->GetShape();

	if (eNum == eCardNumber::N2) return 1;
	if (eNum == eCardNumber::A) {
		return (eShape == eCardShape::S) ? 4 : 2;
	}
	return 0;
}
bool GameScene3::CanDefend(Card* pCard, Card* pAttackingCard) {
	if (!pAttackingCard || !pCard) return false;

	eCardNumber atkNum = pAttackingCard->GetNumber();
	eCardShape  atkShape = pAttackingCard->GetShape();
	eCardNumber defNum = pCard->GetNumber();
	eCardShape  defShape = pCard->GetShape();

	switch (atkNum) {
	case eCardNumber::N2:
		return (defNum == eCardNumber::N3 && defShape == atkShape) ||
			(defNum == eCardNumber::A && defShape == atkShape) ||
			(defNum == eCardNumber::N2 && defShape != atkShape);

	case eCardNumber::A:
		if (atkShape == eCardShape::S)
			return false; // 스페이드A 방어 불가
		return (defNum == eCardNumber::A && defShape != atkShape);
	}
	return false;
}

void GameScene3::DrawCardToPlayer() {
	DrawCard(m_vecPlayerHand, 550.0f);
}
void GameScene3::DrawCardToAI() {
	DrawCard(m_vecAIHand, 50.0f);
}
void GameScene3::DrawCard(std::vector<Card*>& vecHand, float fY) {
	RefillDeckIfEmpty();
	if (m_vecDeck.empty()) return;

	Card* pCard = m_vecDeck.back();
	m_vecDeck.pop_back();
	vecHand.push_back(pCard);

	if (&vecHand == &m_vecAIHand)
		pCard->SetVisible(false); // AI는 뒷면
	else
		pCard->SetVisible(true);  // 플레이어는 앞면

	auto pTransform = pCard->GetComponent<TransformComponent>();
	auto pDeckTransform = m_pCardDeck->GetComponent<TransformComponent>();

	D3DXVECTOR3 vStart = pDeckTransform->GetPosition();
	pTransform->SetPosition(vStart.x, vStart.y, vStart.z);
	pTransform->SetScale(50.0f, 70.0f, 0.0f);

	float fX = 50.0f + (vecHand.size() - 1) * 60.0f;
	pCard->MoveToField(fX, fY, 0.0f);

	m_vecDrawingCards.push_back(pCard); // 여러 장 처리용 벡터에 추가
}

bool GameScene3::UpdateSubmittedCard() {
	if (m_pSubmittedCard && !m_pSubmittedCard->IsMoving()) {
		m_pFieldCard = m_pSubmittedCard;
		m_pSubmittedCard = nullptr;

		if (IsAttackCard(m_pFieldCard)) {
			m_bAttackInProgress = true;
			m_iAttackDrawCount += GetAttackDrawCount(m_pFieldCard);
			m_pAttackingCard = m_pFieldCard;
		}
		else {
			m_bAttackInProgress = false;
			m_iAttackDrawCount = 1;
			m_pAttackingCard = nullptr;
		}

		CheckWinLoseCondition();
		switch (m_pFieldCard->GetNumber()) {
		case eCardNumber::J:
			m_bForceShapeActive = false;
			NextTurn(); // 스킵
			NextTurn(); // 다음 사람 건너뜀
			break;

		case eCardNumber::K:
			m_bForceShapeActive = false;
			// 아무 것도 하지 않으면 자기 턴 유지됨
			break;

		case eCardNumber::N7:
			ChooseShape();
			break;
		default:
			m_bForceShapeActive = false;
			NextTurn(); // 일반 카드 처리
			break;
		}
		m_bAITurnActive = false;
		return true;
	}
	return false;
}

bool GameScene3::UpdateDrawingCard() {
	if (!m_vecDrawingCards.empty()) {
		Card* pCard = m_vecDrawingCards.front();
		if (!pCard->IsMoving()) {
			// 정렬
			if (std::find(m_vecPlayerHand.begin(), m_vecPlayerHand.end(), pCard) != m_vecPlayerHand.end()) {
				ResetPlayerHandPosition();
			}
			else if (std::find(m_vecAIHand.begin(), m_vecAIHand.end(), pCard) != m_vecAIHand.end()) {
				ResetAIHandPosition();
			}

			// 다음 카드로
			m_vecDrawingCards.erase(m_vecDrawingCards.begin());

			// 마지막 카드였으면 드로우 완료 처리
			if (m_vecDrawingCards.empty()) {
				if (m_bAttackInProgress) {
					m_bAttackInProgress = false;
					m_iAttackDrawCount = 1;
					m_pAttackingCard = nullptr;
				}
				CheckWinLoseCondition();
				NextTurn();
			}
		}
		return true; // 현재 드로우 처리 중
	}
	return false;
}

void GameScene3::UpdateAITurn() {
	if (m_pSubmittedCard || m_bAITurnActive) return;

	m_bAITurnActive = true;
	Card* pSelected = nullptr;

	for (Card* pCard : m_vecAIHand) {
		if (!m_bAttackInProgress && IsValidSubmission(pCard)) {
			pSelected = pCard;
			break;
		}
		if (m_bAttackInProgress && CanDefend(pCard, m_pAttackingCard)) {
			pSelected = pCard;
			break;
		}
	}

	if (pSelected) {
		SubmitCard(pSelected, m_vecAIHand);
		ResetAIHandPosition();
	}
	else {
		int iDrawCount = m_bAttackInProgress ? m_iAttackDrawCount : 1;
		for (int i = 0; i < iDrawCount; ++i)
			DrawCardToAI();

		// ✅ 드로우 후 상태 초기화 반드시 수행
		m_bAITurnActive = false;

		m_bAttackInProgress = false;
		m_iAttackDrawCount = 1;
		m_pAttackingCard = nullptr;

		CheckWinLoseCondition();
	}
}

void GameScene3::UpdatePlayerInput() {
	if (m_pSubmittedCard) return;

	auto input = Engine::GetInstance().GetInputSystem();
	if (!input->IsMousePressed()) return;

	POINT pt = ToPOINT(input->GetMousePos());

	if (m_pCardDeck && IsPointInCard(m_pCardDeck, pt)) {
		int iDrawCount = m_bAttackInProgress ? m_iAttackDrawCount : 1;
		for (int i = 0; i < iDrawCount; ++i) {
			DrawCardToPlayer(); // 애니메이션 포함
		}
		m_bAttackInProgress = false;
		m_iAttackDrawCount = 1;
		m_pSelectedCard = nullptr;
		CheckWinLoseCondition();
		return;
	}

	for (Card* pCard : m_vecPlayerHand) {
		if (!IsPointInCard(pCard, pt)) continue;

		if (m_bAttackInProgress && !CanDefend(pCard, m_pAttackingCard))
			break;
		if (!m_bAttackInProgress && !IsValidSubmission(pCard))
			break;

		if (m_pSelectedCard == pCard) {
			SubmitCard(pCard, m_vecPlayerHand);
			ResetPlayerHandPosition();
			m_pSelectedCard = nullptr;
		}
		else {
			if (m_pSelectedCard) {
				auto pPrev = m_pSelectedCard->GetComponent<TransformComponent>();
				pPrev->SetPosition(pPrev->GetPosition().x, 550.0f, 0.0f);
			}
			auto pSel = pCard->GetComponent<TransformComponent>();
			pSel->SetPosition(pSel->GetPosition().x, 550.0f-10.f, 0.0f);
			m_pSelectedCard = pCard;
		}
		break;
	}
}
bool GameScene3::IsValidSubmission(Card* pCard) {
	if (m_bForceShapeActive) {
		return pCard->GetShape() == m_eForcedShape || pCard->GetNumber() == eCardNumber::N7;
	}
	return pCard->GetShape() == m_pFieldCard->GetShape() ||
		pCard->GetNumber() == m_pFieldCard->GetNumber();
}
POINT GameScene3::ToPOINT(const D3DXVECTOR3& vPos) {
	return { (LONG)vPos.x, (LONG)vPos.y };
}
bool GameScene3::IsPointInCard(Object* pObj, const POINT& pt) {
	auto pTransform = pObj->GetComponent<TransformComponent>();
	D3DXVECTOR3 vPos = pTransform->GetPosition();
	D3DXVECTOR3 vScale = pTransform->GetScale();

	RECT rc = {
		(int)(vPos.x - vScale.x * 0.5f),
		(int)(vPos.y - vScale.y * 0.5f),
		(int)(vPos.x + vScale.x * 0.5f),
		(int)(vPos.y + vScale.y * 0.5f)
	};
	return PtInRect(&rc, pt);
}
void GameScene3::SubmitCard(Card* pCard, std::vector<Card*>& vecHand) {
	if (!m_pFieldCard) return;

	auto pTransform = m_pFieldCard->GetComponent<TransformComponent>();
	if (!pTransform) return;

	D3DXVECTOR3 vFieldPos = pTransform->GetPosition();
	pCard->MoveToField(vFieldPos.x, vFieldPos.y, vFieldPos.z);

	m_pSubmittedCard = pCard;
	vecHand.erase(std::remove(vecHand.begin(), vecHand.end(), pCard), vecHand.end());
	pCard->SetVisible(true);
}

void GameScene3::ChooseShape() {
	if (m_eTurn == eTurn::AI) {
		// AI 손패 중 가장 많은 모양 선택
		std::unordered_map<eCardShape, int> mapShapeCount;
		for (Card* pCard : m_vecAIHand)
			mapShapeCount[pCard->GetShape()]++;

		eCardShape eMaxShape = eCardShape::S;
		int iMaxCount = 0;
		for (const auto& [shape, count] : mapShapeCount) {
			if (count > iMaxCount) {
				iMaxCount = count;
				eMaxShape = shape;
			}
		}

		m_eForcedShape = eMaxShape;
		m_bForceShapeActive = true;
		NextTurn();
	}
	else {
		// 플레이어: 버튼 UI 표시만
		m_bChoosingShape = true;
		for (auto& spBtn : m_vecShapeButtons)
			spBtn->SetVisible(true);
	}
}

void GameScene3::OnShapeChosen(eCardShape eShape) {
	m_eForcedShape = eShape;
	m_bForceShapeActive = true;
	m_bChoosingShape = false;

	// 버튼 숨기기
	for (auto& spBtn : m_vecShapeButtons)
		spBtn->SetVisible(false);

	NextTurn();
}

void GameScene3::MakeButtons() {
	std::vector<std::pair<std::wstring, eCardShape>> vecShapeInfo = {
		{L"♠ Spade", eCardShape::S},
		{L"♥ Heart", eCardShape::H},
		{L"♦ Diamond", eCardShape::D},
		{L"♣ Club", eCardShape::C}
	};

	int i = 0;
	for (const auto& [label, shape] : vecShapeInfo) {
		ButtonObject* pBtn = new ButtonObject(objectManager.get(), label, [this, shape]() {
			OnShapeChosen(shape);
			});

		// Transform 설정
		auto tf = pBtn->GetComponent<TransformComponent>();
		tf->SetPosition(200.0f + i * 120.0f, 400.0f, 0.0f); // 하단 정렬
		tf->SetScale(100.0f, 50.0f, 0.0f);

		pBtn->SetVisible(false); // 초기에는 숨김 상태

		// AddObject 및 저장
		std::shared_ptr<ButtonObject> spBtn(pBtn);
		objectManager->AddObject(ObjectType::UI, spBtn);
		m_vecShapeButtons.push_back(spBtn.get());
		++i;
	}
}