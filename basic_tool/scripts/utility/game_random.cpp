#include "game_random.h"

std::mt19937 GameRandom::s_rng{ std::random_device{}() }; // 一時オブジェクトでoperator()を呼び出す
std::mt19937_64 GameRandom::s_rngUUID{ std::random_device{}() };

int GameRandom::GetRange(int min, int max)
{
	std::uniform_int_distribution<int> distribution(min, max);
	return distribution(s_rng);
}

float GameRandom::GetRange(float min, float max)
{
	std::uniform_real_distribution<float> distribution(min, max);
	return distribution(s_rng);
}

uint64_t GameRandom::GetUUID() // unsigned longは32bitのことがあるのでuint64_tを使う
{
	return s_rngUUID(); 
}