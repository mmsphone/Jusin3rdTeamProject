#include "pch.h"
#include "Random.h"

void Random::Initialize()
{
	std::random_device rd;
	Random_gen = std::mt19937(rd());
}

int Random::GetRandom(int min, int max)
{
	std::uniform_int_distribution<int> dis(min, max);
	return dis(Random_gen);
}

float Random::GetRandom(float min, float max)
{
	std::uniform_real_distribution<float> dis(min, max);
	return dis(Random_gen);
}
