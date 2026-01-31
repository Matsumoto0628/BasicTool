#include "window.h"
#include <tchar.h>

// 定数定義
const wchar_t* Window::m_nameWindow = L"ParticleTool";

Window::Window() : m_sizeWindow()
{
	m_sizeWindow.cx = m_width;
	m_sizeWindow.cy = m_height;
}

Window::~Window()
{
    if (m_hWindow) 
    {
        delete m_hWindow;
        m_hWindow = nullptr;
    }
}

bool Window::Initialize(HINSTANCE hInst) 
{
    // ウインドウクラスの登録
    {
        m_wc.style = CS_HREDRAW | CS_VREDRAW;
        m_wc.lpfnWndProc = (WNDPROC)Window::MainWndProc;
        m_wc.cbClsExtra = 0;
        m_wc.cbWndExtra = 0;
        m_wc.hInstance = hInst;
        m_wc.hIcon = LoadIcon(hInst, IDI_APPLICATION);
        m_wc.hCursor = LoadCursor(hInst, IDC_ARROW);
        m_wc.hbrBackground = GetSysColorBrush(COLOR_WINDOW);
        m_wc.lpszMenuName = nullptr;
        m_wc.lpszClassName = m_nameWindow;
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

        m_hWindow = CreateWindow(m_wc.lpszClassName,
            m_nameWindow,
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            rect.right - rect.left,
            rect.bottom - rect.top,
            nullptr,
            nullptr,
            hInst,
            nullptr);

        if (m_hWindow == nullptr) 
        {
            return false;
        }
    }

    // ウインドウ表示
    {
        ShowWindow(m_hWindow, SW_SHOWNORMAL);
        UpdateWindow(m_hWindow);
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