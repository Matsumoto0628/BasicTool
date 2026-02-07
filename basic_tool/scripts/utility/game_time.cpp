#include "game_time.h"

float GameTime::m_deltaTime = 0;
GameTime::TimePoint GameTime::m_prevTime = GameTime::Clock::now();

void GameTime::Update() 
{
	auto now = Clock::now();
	std::chrono::duration<float> elapsed = now - m_prevTime;
	m_deltaTime = elapsed.count();
	m_prevTime = now;
}