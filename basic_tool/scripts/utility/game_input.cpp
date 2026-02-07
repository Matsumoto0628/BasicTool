#include "game_input.h"

bool GameInput::m_prevKeys[KEY_COUNT];
bool GameInput::m_currentKeys[KEY_COUNT];

bool GameInput::GetKeyDown(int key)
{
	return !m_prevKeys[key] && m_currentKeys[key];
}

bool GameInput::GetKeyUp(int key)
{
	return m_prevKeys[key] && !m_currentKeys[key];
}

bool GameInput::GetKey(int key)
{
	return m_currentKeys[key];
}

void GameInput::Update()
{
	for (int i = 0; i < KEY_COUNT; i++) 
	{
		m_prevKeys[i] = m_currentKeys[i];
		m_currentKeys[i] = (GetAsyncKeyState(i) & 0x8000) != 0;
	}
}
