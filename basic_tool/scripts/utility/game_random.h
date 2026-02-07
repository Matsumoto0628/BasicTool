#pragma once
#include <random>

class GameRandom
{
public:
	static int GetRange(int min, int max);
	static float GetRange(float min, float max);

private:
	GameRandom() = delete;
	~GameRandom() = delete;
	static std::mt19937 m_rng; // 乱数エンジン(メルセンヌ・ツイスタ)
};