#pragma once
#include <chrono>

class GameTime
{
	using Clock = std::chrono::high_resolution_clock;
	using TimePoint = Clock::time_point;

public:
	static void Update();
	static float GetDeltaTime() { return s_deltaTime; }

private:
	GameTime() = delete;
	~GameTime() = delete;
	static TimePoint s_prevTime;
	static float s_deltaTime;
};