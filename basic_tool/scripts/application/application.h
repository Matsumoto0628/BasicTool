#pragma once
#include "window.h"
#include <memory>

class RenderContext;
class Scene;

// TODO: コンストラクタを{}にする
// TODO: Componentクラスに純粋仮想関数を宣言して、Params表示関数を定義させる
// TODO: シーンでGuiにgameObjectsを渡して、Guiではcomponentsを参照する
// TODO: Paramsをバイナリで保存する
// TODO: ループ再生を実装
// TODO: Bloomのブラーをカメラからの距離によって変更する
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
};