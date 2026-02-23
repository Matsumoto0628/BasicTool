#include "game_time.h"

float GameTime::s_deltaTime = 0;
GameTime::TimePoint GameTime::s_prevTime = GameTime::Clock::now();

void GameTime::Update() 
{
	auto now = Clock::now();
	std::chrono::duration<float> elapsed = now - s_prevTime;
	s_deltaTime = elapsed.count();
	s_prevTime = now;
}