#include "pch.h"
#include "SoundManager.h"

void SoundManager::Initialize() {
    FMOD::System_Create(&system);
    system->init(512, FMOD_INIT_NORMAL, 0);
}

std::string SoundManager::WStringToString(const std::wstring& wstr) {
    if (wstr.empty()) return std::string();

    int sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
    std::string result(sizeNeeded, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &result[0], sizeNeeded, nullptr, nullptr);

    if (!result.empty() && result.back() == '\0') {
        result.pop_back();
    }

    return result;
}

void SoundManager::InsertSound(const std::wstring& filePath, const std::wstring& key, bool loop) {
    if (mapSound.find(key) != mapSound.end()) return;

    FMOD_MODE mode = loop ? FMOD_LOOP_NORMAL : FMOD_DEFAULT;
    FMOD::Sound* sound = nullptr;

    std::string filePathUtf8 = WStringToString(filePath);

    if (system->createSound(filePathUtf8.c_str(), mode, nullptr, &sound) == FMOD_OK) {
        mapSound[key] = sound;
        std::wcout << L"사운드 삽입 완료: " << key << std::endl;
    }
    else {
        std::wcout << L"사운드 로드 실패: " << filePath << std::endl;
    }
}

void SoundManager::PlaySound(const std::wstring& key, float volume) {
    std::wcout << L"PlaySound 호출된 키: " << key << std::endl;
    auto it = mapSound.find(key);
    if (it == mapSound.end()) {
        std::wcout << L"사운드 키 없음: " << key << std::endl;
        return;
    }

    FMOD::Channel* channel = nullptr;
    system->playSound(it->second, nullptr, false, &channel);
    if (channel) {
        channel->setVolume(volume);
    }
    mapChannel[key] = channel;
}

void SoundManager::StopSound(const std::wstring& key) {
    auto it = mapChannel.find(key);
    if (it != mapChannel.end() && it->second) {
        bool isPlaying = false;
        it->second->isPlaying(&isPlaying);
        if (isPlaying) {
            it->second->stop();
        }

        mapChannel.erase(it);
    }
}

void SoundManager::SetVolume(const std::wstring& key, float volume)
{
    auto it = mapChannel.find(key);
    if (it != mapChannel.end()) {
        FMOD::Channel* channel = it->second;
        if (channel) {
            channel->setVolume(volume);
            std::wcout << L"볼륨 설정 완료: " << key << L" -> " << volume << std::endl;
        }
    }
    else {
        std::wcerr << L"채널 찾기 실패: " << key << std::endl;
    }
}

void SoundManager::Update() {
    if (system) {
        system->update();

        // 채널이 재생 중인지 확인하고, 종료된 채널을 mapChannel에서 제거
        for (auto it = mapChannel.begin(); it != mapChannel.end(); ) {
            bool isPlaying = false;
            it->second->isPlaying(&isPlaying);

            if (!isPlaying) {
                // 사운드가 끝나면 채널 비우기
                it = mapChannel.erase(it); // 채널 제거 후, it를 한 칸 뒤로 이동
            }
            else {
                ++it;
            }
        }
    }
}

void SoundManager::Release() {
    for (auto& pair : mapSound) {
        if (pair.second) {
            pair.second->release();
        }
    }
    mapSound.clear();
    mapChannel.clear();

    if (system) {
        system->close();
        system->release();
        system = nullptr;
    }
}

SoundManager::~SoundManager() {
    Release();
}
