#pragma once
#include <memory>

class RenderContext;

class Gui 
{
public:
	Gui(HWND hWnd, std::shared_ptr<RenderContext> pContext);
	~Gui();
	void Initialize();
	void Start();
	void Update();
	void Draw();
	void Terminate();
	void Finalize();

private:
	Gui() = delete;
	// HWNDを参照として持つのは危険かもしれない
	// HWNDをnullptrにするだけで破棄は任せる
	HWND m_hWnd = nullptr; 
	std::shared_ptr<RenderContext> m_pContext = nullptr;
};