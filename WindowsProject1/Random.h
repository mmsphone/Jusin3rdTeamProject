#pragma once
#include <random>

class Random
{
private:
    std::mt19937 Random_gen;

    // 생성자/소멸자 private
    Random() {
        Initialize();
    }
    ~Random() = default;

public:
    // 복사 방지
    Random(const Random&) = delete;
    Random& operator=(const Random&) = delete;

    static Random& GetInstance() {
        static Random instance;
        static bool initialized = false;
        if (!initialized) {
            instance.Initialize();
            initialized = true;
        }
        return instance;
    }

    void Initialize();

    int GetRandom(int min, int max);
    float GetRandom(float min, float max);
};