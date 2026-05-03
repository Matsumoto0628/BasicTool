/**
 * @file window.h
 * @brief Win32 ウィンドウ管理クラス
 * @author Matsumoto0628
 * @date 2026-05-03
 */
#pragma once
#include <Windows.h>

/**
 * @brief Win32 ウィンドウの生成・メッセージループ・破棄を管理するクラス
 */
class Window
{
public:
    /**
     * @brief コンストラクタ
     * @param hInstance アプリケーションのインスタンスハンドル
     */
    explicit Window(HINSTANCE hInstance);
    ~Window();
    /** @brief ウィンドウを作成して表示する */
    bool Initialize();
    /** @brief ウィンドウリソースを解放する */
    void Finalize();
    /** @brief メッセージをポンプし、WM_QUIT が来たら false を返す */
    bool MessageLoop();
    /**
     * @brief ウィンドウプロシージャ（静的コールバック）
     * @param hWnd   ウィンドウハンドル
     * @param msg    メッセージ識別子
     * @param wParam メッセージパラメータ（ワード）
     * @param lParam メッセージパラメータ（ロング）
     */
    static LRESULT CALLBACK MainWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    /** @brief ウィンドウハンドルを返す */
    HWND GetWindowHandle() const { return m_hWnd; }

private:
    WNDCLASS m_wc;           ///< ウィンドウクラス設定
    HWND m_hWnd = nullptr;   ///< ウィンドウハンドル
    SIZE m_sizeWindow;       ///< ウィンドウサイズ
    MSG m_msg = {};          ///< メッセージ構造体
    HINSTANCE m_hInstance;   ///< アプリケーションインスタンスハンドル

    // 定数
    // インスタンス間で1つなので、staticにして宣言のみ行う
    // 整数型以外や古いコンパイラの場合は定義をソースで行う
    // 配列の数で使うのであれば、enumハックを使う
    static const int WIDTH = 960;
    static const int HEIGHT = 540;
    static const wchar_t* NAME_WINDOW;
};
