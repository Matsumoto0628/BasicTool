#pragma once

class GameInput
{
public:
	static bool GetKeyDown(int key);
	static bool GetKeyUp(int key);
	static bool GetKey(int key);
	static void Update();

private:
	GameInput() = delete;
	~GameInput() = delete;
	const static int KEY_COUNT = 256;
	static bool m_prevKeys[KEY_COUNT];
	static bool m_currentKeys[KEY_COUNT];
};