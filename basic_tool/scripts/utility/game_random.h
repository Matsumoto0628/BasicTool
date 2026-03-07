#pragma once
#include <random>

class GameRandom
{
public:
	static int GetRange(int min, int max);
	static float GetRange(float min, float max);
	static uint64_t GetUUID();

private:
	GameRandom() = delete;
	~GameRandom() = delete;
	static std::mt19937 s_rng; // 乱数エンジン(メルセンヌ・ツイスタ)
	static std::mt19937_64 s_rngUUID;
};