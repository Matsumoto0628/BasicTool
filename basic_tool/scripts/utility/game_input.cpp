#include "game_input.h"

bool GameInput::s_prevKeys[KEY_COUNT];
bool GameInput::s_currentKeys[KEY_COUNT];

bool GameInput::GetKeyDown(int key)
{
	return !s_prevKeys[key] && s_currentKeys[key];
}

bool GameInput::GetKeyUp(int key)
{
	return s_prevKeys[key] && !s_currentKeys[key];
}

bool GameInput::GetKey(int key)
{
	return s_currentKeys[key];
}

void GameInput::Update()
{
	for (int i = 0; i < KEY_COUNT; i++) 
	{
		s_prevKeys[i] = s_currentKeys[i];
		s_currentKeys[i] = (GetAsyncKeyState(i) & 0x8000) != 0;
	}
}
