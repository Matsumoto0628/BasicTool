#pragma once
#include "window.h"
#include <memory>

class RenderContext;
class SceneManager;

// TODO: ビルド(初期データに使っている絶対パスに注意)
// TODO: プリプロセッサ命令でunique_ptrでのメモリ確保を自作アロケータに変更できるようにする(game_objectとcomponent)
// TODO: Spriteアニメーション対応
// TODO: 再生中の色、スケール、回転の変化を実装
// TODO: 3Dメッシュ対応
class Application
{
public:
    explicit Application(HINSTANCE handleInstance); // 引数が1つなので暗黙変換を禁ずる
    ~Application();
    void Initialize(); // 初期化
    void Loop(); // 更新、描画
    void Finalize(); // 破棄

private:
    bool gameLoop();

    Window m_window;
    std::unique_ptr<RenderContext> m_pContext = nullptr;
    std::unique_ptr<SceneManager> m_pSceneManager = nullptr;
};