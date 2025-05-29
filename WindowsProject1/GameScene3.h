#pragma once
class Scene;
#include "Card.h"
class CardDeck;
class ButtonObject;
enum class eTurn {
	Player,
	AI
};

enum class eGameResult {
	None,
	PlayerWin,
	PlayerLose
};

class GameScene3 : public Scene {
private:
	// 카드 상태
	std::vector<Card*> m_vecDeck;
	std::vector<Card*> m_vecPlayerHand;
	std::vector<Card*> m_vecAIHand;

	// 필드 구성 요소
	CardDeck* m_pCardDeck = nullptr;
	Card* m_pFieldCard = nullptr;
	Card* m_pSelectedCard = nullptr;
	Card* m_pSubmittedCard = nullptr;

	// 턴/게임 상태
	eTurn m_eTurn = eTurn::Player;
	eGameResult m_eResult = eGameResult::None;
	bool m_bAITurnActive = false;

	// 공격 처리 관련
	bool m_bAttackInProgress = false;
	int m_iAttackDrawCount = 0;
	Card* m_pAttackingCard = nullptr;

	std::vector<Card*> m_vecDrawingCards;

	bool m_bForceShapeActive = false;
	eCardShape m_eForcedShape;

	std::vector<ButtonObject*> m_vecShapeButtons;
	bool m_bChoosingShape = false;

	bool m_bGameover = false;
	double m_fGameoverElapsed = 0.0;

	bool m_bTurnDelayActive = false;
	double m_fTurnDelayElapsed = 0.0;

	std::wstring m_strTurnText;

	bool m_bShuffling = false;
	double m_fShuffleElapsed = 0.0;
public:
	virtual void Load() override;
	virtual void Update(double dt) override;
	virtual void Render(HDC hdc) override;
	virtual void Unload() override;

private:
	//카드덱 조작
	void ShuffleDeck();
	void RefillDeckIfEmpty();

	// 카드 위치 재배치
	void ResetAIHandPosition();
	void ResetPlayerHandPosition();

	// 게임 승패 조건
	void CheckWinLoseCondition();
	
	//카드 효과
	bool IsAttackCard(Card* pCard);
	bool IsFunctionCard(Card* pCard);
	int GetAttackDrawCount(Card* pCard);
	bool CanDefend(Card* pCard, Card* pAttackingCard);

	// 카드 드로우 애니메이션
	void DrawCardToPlayer();
	void DrawCardToAI();
	void DrawCard(std::vector<Card*>& vecHand, float fY);

	// 턴 처리
	void UpdateAITurn();
	void UpdatePlayerInput();

	bool UpdateSubmittedCard();
	bool UpdateDrawingCard();
	void NextTurn();
	bool IsValidSubmission(Card* pCard);
	POINT ToPOINT(const D3DXVECTOR3& vPos);
	bool IsPointInCard(Object* pObj, const POINT& pt);
	void SubmitCard(Card* pCard, std::vector<Card*>& vecHand);
	void ChooseShape();
	void OnShapeChosen(eCardShape eShape);
	void MakeButtons();
};
