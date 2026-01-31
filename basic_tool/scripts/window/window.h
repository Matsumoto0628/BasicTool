#pragma once
#include <Windows.h>

class Window 
{
public:
	Window(); // 必要に応じてexplicitをつける
	~Window(); // 必要に応じてvirtualをつける
	bool Initialize(HINSTANCE hInst);
	void Finalize();
	bool MessageLoop();
	static LRESULT CALLBACK MainWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	const HWND GetWindowHandle() const { return m_hWindow; }

private:
	WNDCLASS m_wc;
	HWND m_hWindow = nullptr;
	SIZE m_sizeWindow;
	MSG m_msg = {};

	// 定数
	// インスタンス間で1つなので、staticにして宣言のみ行う
	// 整数型以外や古いコンパイラの場合は定義をソースで行う
	// 配列の数で使うのであれば、enumハックを使う
	static const int WIDTH = 960;
	static const int HEIGHT = 540;
	static const wchar_t* NAME_WINDOW;
};