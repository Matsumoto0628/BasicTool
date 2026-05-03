/**
 * @file test_scene.h
 * @brief テストシーン
 * @author Matsumoto0628
 * @date 2026-05-03
 */
#pragma once
#include "scene.h"

class RenderContext;
class Gui;

/**
 * @brief 動作確認用のテストシーン
 *
 * GUI・カメラ・パーティクルシステムを配置した開発用シーン。
 */
class TestScene : public Scene
{
public:
    /**
     * @brief コンストラクタ
     * @param hWnd     ウィンドウハンドル
     * @param pContext レンダリングコンテキスト
     */
    TestScene(HWND hWnd, RenderContext* const pContext);
    ~TestScene();
    void Initialize() override;
    void Setup() override;
    void Start() override;
    void Update() override;
    void Draw() override;
    void Terminate() override;
    void Finalize() override;

private:
    TestScene() = delete;
    std::unique_ptr<Gui> m_pGui = nullptr; ///< エディター GUI
};
