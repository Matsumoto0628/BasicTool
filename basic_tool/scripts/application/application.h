#pragma once
#include "window.h"
#include <memory>

class RenderContext;
class Renderable;

class Application
{
public:
    explicit Application(HINSTANCE handleInstance);
    ~Application();
    void Initialize(); // 初期化
    void Start(); // 開始
    void Loop(); // 更新、描画
    void Terminate(); // 終了
    void Finalize(); // 破棄

private:
    bool gameLoop();

private:
    Window m_window; // 寿命がApplicationと同じなので実体
    std::shared_ptr<RenderContext> m_pRenderContext; // 共有したいのでポインタ
    std::unique_ptr<Renderable> m_pRenderable; // ポリモーフィズムしたいのでポインタ
};