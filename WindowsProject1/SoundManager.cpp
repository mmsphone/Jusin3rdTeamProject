#include "pch.h"
#include "SoundManager.h"

SoundManager::SoundManager() {
    FMOD_RESULT result = FMOD::System_Create(&system);
    if (result != FMOD_OK || system == nullptr) {
        std::cout << "FMOD System_Create 실패: " << FMOD_ErrorString(result) << std::endl;
        return;
    }    
    system->init(512, FMOD_INIT_NORMAL, 0);
}

SoundManager::~SoundManager() {
    for (auto& [_, sound] : sounds) {
        sound->release();
    }
    system->close();
    system->release();
}


bool SoundManager::LoadSound(const std::string& name, const std::string& filePath, bool loop, double volume) {
    FMOD_MODE mode = loop ? FMOD_LOOP_NORMAL : FMOD_DEFAULT;
    FMOD::Sound* sound = nullptr;

    // 오류 코드 확인
    FMOD_RESULT result = system->createSound(filePath.c_str(), mode, nullptr, &sound);
    if (result == FMOD_OK) {
        sounds[name] = sound;
        SetVolume(name, volume);
        return true;
    }
    else {
        std::cout << "FMOD Error: " << FMOD_ErrorString(result) << std::endl;
        return false;
    }
}
void SoundManager::StartSound(const std::string& name, bool record) {
    if (sounds.count(name)) {
        system->playSound(sounds[name], 0, false, &channels[name]);

        if (volumeMap.count(name)) {
            channels[name]->setVolume(volumeMap[name]);
        }

    }
    else
    {
        std::cout << "Can't find sound: " + name << std::endl;
    }
}

void SoundManager::SetPlaySpeed(const std::string& name, float rate)
{
    if (channels.count(name)) {
        float baseFreq = 0.0f;

        // 초기화된 적 없으면 저장
        if (!originalFrequencies.count(name)) {
            channels[name]->getFrequency(&baseFreq);
            originalFrequencies[name] = baseFreq;
        }
        else {
            baseFreq = originalFrequencies[name];
        }

        channels[name]->setFrequency(baseFreq * rate);
    }
}

void SoundManager::StopSound(const std::string& name, bool record) {
    if (channels.count(name)) {
        channels[name]->stop();
    }
}

void SoundManager::SetVolume(const std::string& name, float volume) {
    volumeMap[name] = volume;

    if (channels.count(name)) {
        channels[name]->setVolume(volume);
    }
}
void SoundManager::Update() {
    system->update();
}

void SoundManager::PauseSound(const std::string& name) {
    if (channels.count(name)) {
        bool isPlaying = false;
        channels[name]->isPlaying(&isPlaying);
        if (isPlaying) {
            channels[name]->setPaused(true);
        }
    }
}

void SoundManager::ResumeSound(const std::string& name) {
    if (channels.count(name)) {
        bool isPaused = false;
        channels[name]->getPaused(&isPaused);
        if (isPaused) {
            channels[name]->setPaused(false);
        }
    }
}