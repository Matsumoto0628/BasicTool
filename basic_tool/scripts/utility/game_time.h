#pragma once
#include <chrono>

class GameTime
{
	using Clock = std::chrono::high_resolution_clock;
	using TimePoint = Clock::time_point;

public:
	static void Update();
	static float GetDeltaTime() { return m_deltaTime; }

private:
	GameTime() = delete;
	~GameTime() = delete;
	static TimePoint m_prevTime;
	static float m_deltaTime;
};