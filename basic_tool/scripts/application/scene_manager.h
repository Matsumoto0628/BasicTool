/**
 * @file scene_manager.h
 * @brief シーン管理クラス
 * @author Matsumoto0628
 * @date 2026-05-03
 */
#pragma once
#include "scene.h"
#include <map>
#include <memory>

class RenderContext;

/**
 * @brief 複数のシーンを管理し、シーン遷移を行うクラス
 */
class SceneManager
{
public:
    /**
     * @brief コンストラクタ
     * @param hWnd     ウィンドウハンドル
     * @param pContext レンダリングコンテキスト
     */
    SceneManager(HWND hWnd, RenderContext* const pContext);
    ~SceneManager();
    /** @brief 全シーンを生成して初期シーンを設定する */
    void Initialize();
    /** @brief 現在シーンの更新処理を実行する */
    void Update();
    /** @brief 現在シーンの描画処理を実行する */
    void Draw();
    /** @brief 全シーンのリソースを解放する */
    void Finalize();

    /** @brief シーンの種別を表す列挙型 */
    enum class SceneType
    {
        Test,
    };
    /**
     * @brief 指定したシーンに遷移する
     * @param sceneType 遷移先シーンの種別
     */
    static void Transit(SceneType sceneType);
    /** @brief 現在アクティブなシーンへのポインタを返す */
    static Scene* const GetCurrentScene() { return s_pCurrentScene; }

private:
    SceneManager() = delete;
    static Scene*                                        s_pCurrentScene; ///< 現在のシーン
    static std::map<SceneType, std::unique_ptr<Scene>>   s_pScenes;       ///< 全シーンマップ
};
