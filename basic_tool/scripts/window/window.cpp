#include "window.h"
#include <tchar.h>
#include "imgui_impl_win32.h"

// 定数定義
const wchar_t* Window::NAME_WINDOW = L"ParticleTool";

// ヘッダ汚染を防ぐため自分で前方宣言が必要
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

Window::Window(HINSTANCE hInstance) 
    : m_sizeWindow(), m_hInstance(hInstance)
{
	m_sizeWindow.cx = WIDTH;
	m_sizeWindow.cy = HEIGHT;
}

Window::~Window()
{
    if (m_hWnd) 
    {
        m_hWnd = nullptr;
    }
}

bool Window::Initialize() 
{
    // ウインドウクラスの登録
    {
        m_wc.style = CS_HREDRAW | CS_VREDRAW;
        m_wc.lpfnWndProc = (WNDPROC)Window::MainWndProc;
        m_wc.cbClsExtra = 0;
        m_wc.cbWndExtra = 0;
        m_wc.hInstance = m_hInstance;
        m_wc.hIcon = LoadIcon(m_hInstance, IDI_APPLICATION);
        m_wc.hCursor = LoadCursor(m_hInstance, IDC_ARROW);
        m_wc.hbrBackground = GetSysColorBrush(COLOR_WINDOW);
        m_wc.lpszMenuName = nullptr;
        m_wc.lpszClassName = NAME_WINDOW;
        if (!RegisterClass(&m_wc)) 
        {
            return false;
        }
    }

    // メインウインドウ作成
    {
        RECT rect;
        rect.top = 0;
        rect.left = 0;
        rect.right = m_sizeWindow.cx;
        rect.bottom = m_sizeWindow.cy;
        AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

        m_hWnd = CreateWindow(
            m_wc.lpszClassName,
            NAME_WINDOW,
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            rect.right - rect.left,
            rect.bottom - rect.top,
            nullptr,
            nullptr,
            m_hInstance,
            nullptr
        );

        if (m_hWnd == nullptr) 
        {
            return false;
        }
    }

    // ウインドウ表示
    {
        ShowWindow(m_hWnd, SW_SHOWNORMAL);
        UpdateWindow(m_hWnd);
    }

    return true;
}

void Window::Finalize()
{
    DestroyWindow(GetWindowHandle());

    // ウインドウクラスの登録解除
    UnregisterClass(m_wc.lpszClassName, m_wc.hInstance);
}

bool Window::MessageLoop()
{
    // Windowsメッセージループ処理
    if (PeekMessage(&m_msg, nullptr, 0, 0, PM_REMOVE)) 
    {
        TranslateMessage(&m_msg); 
        DispatchMessage(&m_msg);
    }

    // アプリケーションが終わるときにmessageがWM_QUITになる 
    if (m_msg.message == WM_QUIT) 
    {
        return false;
    }

    return true;
}

LRESULT CALLBACK Window::MainWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    // GUIの操作
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam)) 
    {
        return true;
    }

    switch (msg) 
    {
    case WM_DESTROY:
        // アプリ終了
        PostQuitMessage(0);
        return 0;
    }

    // デフォルト処理
    return DefWindowProc(hWnd, msg, wParam, lParam);
}