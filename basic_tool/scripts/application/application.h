#pragma once
#include "window.h"
#include <memory>

class RenderContext;
class Scene;
class Gui;
class Camera;

// TODO: HDR&発光
// TODO: imguiで操作を反映する
class Application
{
public:
    explicit Application(HINSTANCE handleInstance); // 引数が1つなので暗黙的変換を禁ずる
    ~Application();
    void Initialize(); // 初期化
    void Start(); // 開始
    void Loop(); // 更新、描画
    void Terminate(); // 終了
    void Finalize(); // 破棄

private:
    bool gameLoop();

    Window m_window;
    std::unique_ptr<RenderContext> m_pContext = nullptr;
    std::unique_ptr<Scene> m_pScene = nullptr;
    std::unique_ptr<Gui> m_pGui = nullptr;
};