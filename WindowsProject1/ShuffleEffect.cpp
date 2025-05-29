#include "pch.h"
#include "ShuffleEffect.h"
#include "TransformComponent.h"
#include <cmath>

ShuffleEffect::ShuffleEffect(ObjectManager* pOwner, int iCardCount, const D3DXVECTOR3& vStart, const D3DXVECTOR3& vEnd)
    : Effect(pOwner, ObjectType::Effect), m_vStart(vStart), m_vEnd(vEnd) {

    lifetime = m_fTotalTime;

    for (int i = 0; i < iCardCount; ++i) {
        DummyCard card;
        card.pTransform = std::make_unique<TransformComponent>();
        card.pTransform->SetPosition(vStart.x, vStart.y, 0.0f);
        m_vecCards.push_back(std::move(card));
    }
}
void ShuffleEffect::Update(double dt) {
    Effect::Update(dt);
    float t = static_cast<float>(elapsedTime);

    const float fMoveLeftEnd = 1.0f;
    const float fRotate3DEnd = 1.5f;
    const float fShuffleEnd = 3.0f;
    const float fReturn2DEnd = 3.5f;
    const float fMoveBackEnd = 5.0f;

    D3DXVECTOR3 vShuffleBasePos = m_vStart;
    vShuffleBasePos.x -= 250.0f;

    int iCardCount = static_cast<int>(m_vecCards.size());
    int iTopCount = iCardCount * 3 / 10;
    int iBottomCount = iCardCount * 3 / 10;
    int iMiddleCount = iCardCount - iTopCount - iBottomCount;

    int iTopStart = 0;
    int iTopEnd = iTopStart + iTopCount - 1;
    int iMiddleStart = iTopEnd + 1;
    int iMiddleEnd = iMiddleStart + iMiddleCount - 1;
    int iBottomStart = iMiddleEnd + 1;
    int iBottomEnd = iCardCount - 1;

    const float fZGap = 2.0f; // 카드 간 z축 간격
    const float rotateX = 30.f;
    const float rotateZ = -30.f;

    if (t < fMoveLeftEnd) {
        float ratio = t / fMoveLeftEnd;
        float targetX = m_vStart.x - 250.0f;

        for (int i = 0; i < iCardCount; ++i) {
            float x = (1 - ratio) * m_vStart.x + ratio * targetX;
            float z = static_cast<float>(iCardCount - i) * fZGap;
            m_vecCards[i].pTransform->SetPosition(x, m_vStart.y, z);
            m_vecCards[i].pTransform->SetRotationX(0.0f);
            m_vecCards[i].pTransform->SetRotationY(0.0f);
            m_vecCards[i].pTransform->SetRotationZ(0.0f);
        }
    }
    else if (t < fRotate3DEnd) {
        float ratio = (t - fMoveLeftEnd) / (fRotate3DEnd - fMoveLeftEnd);
        float angleX = D3DXToRadian(rotateX) * ratio;
        float angleZ = D3DXToRadian(rotateZ) * ratio;

        for (int i = 0; i < iCardCount; ++i) {
            float z = static_cast<float>(iCardCount - i) * fZGap;
            m_vecCards[i].pTransform->SetPosition(vShuffleBasePos.x, m_vStart.y, z);
            m_vecCards[i].pTransform->SetRotationX(angleX);
            m_vecCards[i].pTransform->SetRotationY(0.0f);
            m_vecCards[i].pTransform->SetRotationZ(angleZ);
        }
    }
    else if (t < fShuffleEnd) {
        float shuffleT = (t - fRotate3DEnd) / (fShuffleEnd - fRotate3DEnd);

        for (int i = 0; i < iCardCount; ++i) {
            float z = static_cast<float>(iCardCount - i) * fZGap;
            float x = vShuffleBasePos.x;
            float y = m_vStart.y;

            if (i >= iMiddleStart && i <= iMiddleEnd) {
                // 중앙 카드: 위로 빼기 → 다시 덮기
                if (shuffleT < 0.33f) {
                    float ratio = shuffleT / 0.33f;
                    y -= 60.0f * ratio;
                    x += 30.0f * ratio;
                }
                else if (shuffleT < 0.66f) {
                    y -= 60.0f;
                    x += 30.0f;
                }
                else {
                    float ratio = (shuffleT - 0.66f) / 0.34f;
                    y = (m_vStart.y - 60.0f) + (75.0f * ratio);
                    x = (vShuffleBasePos.x + 30.0f) - (30.0f * ratio);
                }
            }
            else {
                if (i <= iTopEnd) {
                    // 상단 카드: 중반에 하단 위치로 내려옴
                    if (shuffleT > 0.33f) {
                        float ratio = std::min((shuffleT - 0.33f) / 0.33f, 1.0f);
                        y += 15.0f * ratio;
                    }
                }
                else if (i >= iBottomStart) {
                    // 하단은 위치 고정
                }
            }

            m_vecCards[i].pTransform->SetPosition(x, y, z);
        }
    }
    else if (t < fReturn2DEnd) {
        float ratio = (t - fShuffleEnd) / (fReturn2DEnd - fShuffleEnd);
        float angleX = D3DXToRadian(rotateX) * (1.0f - ratio);
        float angleZ = D3DXToRadian(rotateZ) * (1.0f - ratio);

        for (auto& card : m_vecCards) {
            card.pTransform->SetRotationX(angleX);
            card.pTransform->SetRotationY(0.0f);
            card.pTransform->SetRotationZ(angleZ);
        }
    }
    else {
        float ratio = (t - fReturn2DEnd) / (fMoveBackEnd - fReturn2DEnd);
        if (ratio > 1.0f) ratio = 1.0f;

        float fromX = m_vStart.x - 250.0f;
        float fromY = m_vStart.y;

        for (int i = 0; i < iCardCount; ++i) {
            float x = (1 - ratio) * fromX + ratio * m_vEnd.x;
            float y = (1 - ratio) * fromY + ratio * m_vEnd.y;
            float z = static_cast<float>(iCardCount - i) * fZGap * (1.0f - ratio);

            m_vecCards[i].pTransform->SetPosition(x, y, z);
            m_vecCards[i].pTransform->SetRotationX(0.0f);
            m_vecCards[i].pTransform->SetRotationY(0.0f);
            m_vecCards[i].pTransform->SetRotationZ(0.0f);
        }
    }
}

void ShuffleEffect::Render(HDC hdc) {
    // 카드 개수만큼 인덱스 배열 생성
    std::vector<int> vecIndex(m_vecCards.size());
    for (int i = 0; i < static_cast<int>(m_vecCards.size()); ++i)
        vecIndex[i] = i;

    // Z값 기준 인덱스 정렬
    std::sort(vecIndex.begin(), vecIndex.end(), [&](int a, int b) {
        return m_vecCards[a].pTransform->GetPosition().z < m_vecCards[b].pTransform->GetPosition().z;
        });

    // 정렬된 인덱스 순서대로 그리기
    for (int idx : vecIndex) {
        const auto& card = m_vecCards[idx];

        float halfW = card.fWidth * 0.5f;
        float halfH = card.fHeight * 0.5f;

        D3DXVECTOR3 localCorners[4] = {
            { -halfW, -halfH, 0 },
            {  halfW, -halfH, 0 },
            {  halfW,  halfH, 0 },
            { -halfW,  halfH, 0 }
        };

        const D3DXMATRIX& mat = card.pTransform->GetWorldMatrix();
        POINT points[4];

        const float fPerspective = 0.5f;

        for (int j = 0; j < 4; ++j) {
            D3DXVECTOR3 transformed;
            D3DXVec3TransformCoord(&transformed, &localCorners[j], &mat);

            // 카드 회전X을 기준으로 Z를 Y에 보정
            float zOffsetY = transformed.z * sinf(card.pTransform->GetRotationX());
            points[j].x = static_cast<LONG>(transformed.x);
            points[j].y = static_cast<LONG>(transformed.y - zOffsetY);
        }

        Polygon(hdc, points, 4);
    }
}
