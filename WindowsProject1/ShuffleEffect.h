#pragma once
#include "Effect.h"
#include <vector>
#include <d3dx9math.h>
class TransformComponent;
class ShuffleEffect : public Effect {
private:
    struct DummyCard {
        std::unique_ptr<TransformComponent> pTransform = nullptr;
        float fWidth = 50.0f;
        float fHeight = 70.0f;
    };

    std::vector<DummyCard> m_vecCards;
    D3DXVECTOR3 m_vStart;
    D3DXVECTOR3 m_vEnd;
    float m_fTotalTime = 5.0f;

    float m_fMoveLeftTime = 1.0f;
    float m_fShuffleTime = 2.0f;
    float m_fReturn2DTime = 1.0f;
    float m_fMoveBackTime = 1.0f;

    //float m_fTimeLeftEnd;
    //float m_fTimeShuffleEnd;
    //float m_fTimeReturnEnd;
    //
    //bool m_bShuffled = false;
    //
    //float m_fShuffleStartTime = 1.0f;
    //float m_fShuffleEndTime = 2.5f;

public:
    ShuffleEffect(ObjectManager* pOwner, int iCardCount, const D3DXVECTOR3& vStart, const D3DXVECTOR3& vEnd);

    void Update(double dt) override;
    void Render(HDC hdc) override;
};
