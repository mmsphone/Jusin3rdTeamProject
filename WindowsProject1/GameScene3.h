#pragma once
class Scene;
class Card;
class CardDeck;
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
	std::vector<Card*> m_vecDeck;
	std::vector<Card*> m_vecPlayerHand;
	std::vector<Card*> m_vecAIHand;
	CardDeck* m_pCardDeck = nullptr;
	Card* m_pFieldCard = nullptr;
	Card* m_pSelectedCard = nullptr;
	Card* m_pSubmittedCard = nullptr;
	eTurn m_eTurn = eTurn::Player;
	bool m_bAITurnStarted = false;
	eGameResult m_eResult = eGameResult::None;
	bool m_bAttackInProgress = false;         // 공격 진행 중 여부
	int m_iAttackDrawCount = 0;               // 누적된 공격 드로우 수
	Card* m_pAttackingCard = nullptr;         // 공격 트리거가 된 카드
public:
	virtual void Load() override;
	virtual void Update(double dt) override;
	virtual void Render(HDC hdc) override;
	virtual void Unload() override;

	void NextTurn();
	void ResetAIHandPosition();
	void ResetPlayerHandPosition();
	void CheckWinLoseCondition();
	bool IsAttackCard(Card* pCard);
	int GetAttackDrawCount(Card* pCard);
	bool CanDefend(Card* pCard, Card* pAttackingCard);
};
