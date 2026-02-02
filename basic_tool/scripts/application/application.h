#pragma once
#include "window.h"
#include <memory>

class RenderContext;
class Renderable;
class Gui;
class Camera;

// TODO: Sceneクラスの実装
// TODO: GetRenderable()でここで一気に描画
// TODO: RenderableクラスでGetTransform()して操作する
// TODO: InputManagerで操作できるようにする
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
    std::unique_ptr<RenderContext> m_pRenderContext = nullptr;
    std::unique_ptr<Renderable> m_pRenderable = nullptr;
    std::unique_ptr<Gui> m_pGui = nullptr;
    std::unique_ptr<Camera> m_pCamera = nullptr;
};