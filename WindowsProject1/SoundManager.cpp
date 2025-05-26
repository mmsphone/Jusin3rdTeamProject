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
        std::wcout << L"���� ���� �Ϸ�: " << key << std::endl;
    }
    else {
        std::wcout << L"���� �ε� ����: " << filePath << std::endl;
    }
}

void SoundManager::PlaySound(const std::wstring& key, float volume) {
    std::wcout << L"PlaySound ȣ��� Ű: " << key << std::endl;
    auto it = mapSound.find(key);
    if (it == mapSound.end()) {
        std::wcout << L"���� Ű ����: " << key << std::endl;
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
            std::wcout << L"���� ���� �Ϸ�: " << key << L" -> " << volume << std::endl;
        }
    }
    else {
        std::wcerr << L"ä�� ã�� ����: " << key << std::endl;
    }
}

void SoundManager::Update() {
    if (system) {
        system->update();

        // ä���� ��� ������ Ȯ���ϰ�, ����� ä���� mapChannel���� ����
        for (auto it = mapChannel.begin(); it != mapChannel.end(); ) {
            bool isPlaying = false;
            it->second->isPlaying(&isPlaying);

            if (!isPlaying) {
                // ���尡 ������ ä�� ����
                it = mapChannel.erase(it); // ä�� ���� ��, it�� �� ĭ �ڷ� �̵�
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
