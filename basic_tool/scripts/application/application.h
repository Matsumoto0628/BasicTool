#pragma once
#include "window.h"
#include <memory>

class RenderContext;
class Scene;
class Gui;
class Camera;

// TODO: 親子関係の回転を実装する
// TODO: カメラとSphereの最初の移動を辞める
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

    Window m_window;
    std::unique_ptr<RenderContext> m_pContext = nullptr;
    std::unique_ptr<Scene> m_pScene = nullptr;
    std::unique_ptr<Gui> m_pGui = nullptr;
    std::unique_ptr<Camera> m_pCamera = nullptr;
};