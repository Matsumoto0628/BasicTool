#pragma once
#include "window.h"
#include <memory>

class RenderContext;
class SceneManager;

// TODO: 画像をスプライトシートで出力
// TODO: オブジェクト生成、削除をGUI上で実装
// TODO: パーティクルの画像変更、動き、寿命を変更できるようにする
// TODO: プリプロセッサ命令でunique_ptrでのメモリ確保を自作アロケータに変更できるようにする(game_objectとcomponent)
// TODO: Bloomのブラーをカメラからの距離(大きさ)によって変更する
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