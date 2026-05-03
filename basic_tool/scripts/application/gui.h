/**
 * @file gui.h
 * @brief ImGui を使ったエディター GUI クラス
 * @author Matsumoto0628
 * @date 2026-05-03
 */
#pragma once
#include <string>
#include <vector>
#include <memory>

class RenderContext;
class GameObject;

/**
 * @brief ImGui を使ってヒエラルキー・インスペクター・メニューを表示するエディター GUI クラス
 */
class Gui
{
public:
    /**
     * @brief コンストラクタ
     * @param hWnd           ウィンドウハンドル
     * @param pContext       レンダリングコンテキスト
     * @param ppGameObjects  シーン上のゲームオブジェクトリスト
     */
    Gui(HWND hWnd, const RenderContext* const pContext, const std::vector<std::unique_ptr<GameObject>>* ppGameObjects);
    ~Gui();
    /** @brief ImGui コンテキストを初期化する */
    void Initialize();
    /** @brief GUI の描画を更新する（毎フレーム呼び出し） */
    void Update();
    /** @brief ImGui リソースを解放する */
    void Finalize();

private:
    Gui() = delete;
    void drawMainMenu();
    void drawHierarchy();
    void drawInspector();
    void drawHierarchyNode(GameObject* pGameObject);
    void drawSerializePopup();
    std::string openFileDialog();
    void checkOpenFile();
    void checkExit();
    void drawExportPopup();
    void drawPlaybackControl();

    HWND                                                   m_hWnd       = nullptr; ///< ウィンドウハンドル
    const RenderContext* const                             m_pContext   = nullptr; ///< レンダリングコンテキスト
    const std::vector<std::unique_ptr<GameObject>>*        m_ppGameObjects;        ///< ゲームオブジェクトリスト
    const GameObject*                                      m_pSelectGameObject = nullptr; ///< 選択中オブジェクト
    char  m_serializeFileName[256] = "scene";  ///< シリアライズファイル名バッファ
    char  m_exportFileName[256]    = "sheet";  ///< エクスポートファイル名バッファ
    int   m_exportNumW             = 3;        ///< スプライトシート横分割数
    int   m_exportNumH             = 3;        ///< スプライトシート縦分割数
    bool  m_openPopupRequestd      = false;    ///< ポップアップ表示要求フラグ
    bool  m_openFileRequested      = false;    ///< ファイルオープン要求フラグ
    bool  m_exitRequested          = false;    ///< 終了要求フラグ
    bool  m_openExportRequested    = false;    ///< エクスポートダイアログ要求フラグ
    bool  m_isPause                = false;    ///< 再生一時停止フラグ
};
