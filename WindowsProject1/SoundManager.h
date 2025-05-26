#pragma once

class SoundManager {
public:
    SoundManager(const SoundManager&) = delete;
    SoundManager& operator=(const SoundManager&) = delete;
    SoundManager(SoundManager&&) = delete;
    SoundManager& operator=(SoundManager&&) = delete;

    void Initialize();
    void InsertSound(const std::wstring& filePath, const std::wstring& key, bool loop = false);
    void PlaySound(const std::wstring& key, float volume = 1.f);
    void StopSound(const std::wstring& key);
    void SetVolume(const std::wstring& key, float volume);
    void Update();
    void Release();

private:
    SoundManager() noexcept = default;
    ~SoundManager();

    FMOD::System* system = nullptr;
    std::unordered_map<std::wstring, FMOD::Sound*> mapSound;
    std::unordered_map<std::wstring, FMOD::Channel*> mapChannel;

    std::string WStringToString(const std::wstring& wstr); // 유니코드 변환 함수
};
