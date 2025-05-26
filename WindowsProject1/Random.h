#pragma once
#include <random>

class Random
{
private:
    std::mt19937 Random_gen;

    // ������/�Ҹ��� private
    Random() = default;
    ~Random() = default;

public:
    // ���� ����
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