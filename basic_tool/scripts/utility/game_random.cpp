#include "game_random.h"

std::mt19937 GameRandom::m_rng{ std::random_device{}() };

int GameRandom::GetRange(int min, int max)
{
	std::uniform_int_distribution<int> distribution(min, max);
	return distribution(m_rng);
}

float GameRandom::GetRange(float min, float max)
{
	std::uniform_real_distribution<float> distribution(min, max);
	return distribution(m_rng);
}