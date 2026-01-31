#pragma once
#include "window.h"

class Application
{
public:
    void Initialize(HINSTANCE hInst); // 初期化
    void Start(); // 開始
    void Loop(); // 更新、描画
    void Terminate(); // 終了
    void Finalize(); // 破棄

private:
    bool gameLoop();

private:
    Window m_window;
};