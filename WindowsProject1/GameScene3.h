#pragma once
class Scene;
class Card;

class GameScene3 : public Scene {
private:
	std::vector<Card*> m_vecDeck;
	std::vector<Card*> m_vecPlayerHand;
	std::vector<Card*> m_vecAIHand;
	Card* m_pFieldCard = nullptr;
	Card* m_pSelectedCard = nullptr;
	Card* m_pSubmittedCard = nullptr;
public:
	virtual void Load() override;
	virtual void Update(double dt) override;
	virtual void Render(HDC hdc) override;
	virtual void Unload() override;
};
