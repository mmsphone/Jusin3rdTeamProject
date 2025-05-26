#pragma once
#include "ObjectComponent.h"

class AnimationComponent : public ObjectComponent {
public:
    AnimationComponent() = default;
    virtual ~AnimationComponent() override = default;

    virtual void Initialize() override {}
    virtual void Update(double dt) override;

    void AddFrame(const std::wstring& textureKey);
    void SetFPS(double fps);
    void SetLoop(bool loop);
    void Play();
    void Stop();
    bool IsPlaying() const;

    void Render(HDC hdc);

private:
    std::vector<std::wstring> vecFrames;
    int iCurrentFrame = 0;
    double fTimeAccumulator = 0.0;
    double fFrameTime = 0.1;
    bool bLoop = true;
    bool bIsPlaying = true;
};
