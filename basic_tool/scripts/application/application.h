/**
 * @file application.h
 * @brief アプリケーション管理クラス
 * @author Matsumoto0628
 * @date 2026-05-03
 */
#pragma once
#include "window.h"
#include <memory>

class RenderContext;
class SceneManager;

// TODO: プリプロセッサ命令でunique_ptrでのメモリ確保を自作アロケータに変更できるようにする(game_objectとcomponent)
// TODO: Spriteアニメーション対応
// TODO: 再生中の色、スケール、回転の変化を実装
// TODO: 3Dメッシュ対応

/**
 * @brief アプリケーション全体のライフサイクルを管理するクラス
 *
 * ウィンドウ・レンダリングコンテキスト・シーンマネージャーを保持し、
 * Initialize → Loop → Finalize の順で呼び出して使用する。
 */
class Application
{
public:
    /**
     * @brief コンストラクタ
     * @param handleInstance アプリケーションのインスタンスハンドル（暗黙変換禁止）
     */
    explicit Application(HINSTANCE handleInstance);
    ~Application();
    /** @brief ウィンドウ・DirectX デバイス・シーンを初期化する */
    void Initialize();
    /** @brief メインループを実行する（更新・描画を繰り返す） */
    void Loop();
    /** @brief 全リソースを解放する */
    void Finalize();

private:
    bool gameLoop();

    Window                          m_window;
    std::unique_ptr<RenderContext>  m_pContext      = nullptr; ///< DirectX11 レンダリングコンテキスト
    std::unique_ptr<SceneManager>   m_pSceneManager = nullptr; ///< シーン管理
};
