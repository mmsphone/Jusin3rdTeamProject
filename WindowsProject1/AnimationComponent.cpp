#include "pch.h"
#include "AnimationComponent.h"
#include "TransformComponent.h"
#include "TextureManager.h"
#include "Engine.h"
#include "Object.h"

void AnimationComponent::Update(double dt) {
    if (!bIsPlaying || vecFrames.empty()) return;

    fTimeAccumulator += dt;
    if (fTimeAccumulator >= fFrameTime) {
        fTimeAccumulator -= fFrameTime;
        iCurrentFrame++;

        if (iCurrentFrame >= (int)vecFrames.size()) {
            if (bLoop) {
                iCurrentFrame = 0;
            }
            else {
                iCurrentFrame = (int)vecFrames.size() - 1;
                bIsPlaying = false;
            }
        }
    }
}

void AnimationComponent::Render(HDC hdc) {
    if (vecFrames.empty()) return;

    auto transform = owner->GetComponent<TransformComponent>();
    if (!transform) return;

    const std::wstring& sKey = vecFrames[iCurrentFrame];
    auto* pTextureMgr = Engine::GetInstance().GetTextureManager();

    HDC hFrameDC = pTextureMgr->FindTexture(sKey);
    if (!hFrameDC) return;

    D3DXVECTOR3 tSize = pTextureMgr->GetTextureSize(sKey);
    if (tSize.x == 0 || tSize.y == 0) return;

    D3DXVECTOR3 vPos = transform->GetPosition();

    BitBlt(
        hdc,
        static_cast<int>(vPos.x - tSize.x * 0.5f),
        static_cast<int>(vPos.y - tSize.y * 0.5f),
        tSize.x,
        tSize.y,
        hFrameDC,
        0, 0,
        SRCCOPY
    );
}


void AnimationComponent::AddFrame(const std::wstring& textureKey) {
    vecFrames.push_back(textureKey);
}

void AnimationComponent::SetFPS(double fps) {
    fFrameTime = 1.0 / fps;
}

void AnimationComponent::SetLoop(bool loop) {
    bLoop = loop;
}

void AnimationComponent::Play() {
    bIsPlaying = true;
    iCurrentFrame = 0;
    fTimeAccumulator = 0.0;
}

void AnimationComponent::Stop() {
    bIsPlaying = false;
}

bool AnimationComponent::IsPlaying() const {
    return bIsPlaying;
}
