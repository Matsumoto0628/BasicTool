#include "application.h"

Application::Application(HINSTANCE hInstance)
    : m_window(hInstance)
{
}

Application::~Application() 
{
}

void Application::Initialize()
{
    m_window.Initialize();
}

void Application::Start()
{
}

void Application::Loop()
{
    while (true) 
    {
        // Windowsのメッセージループ
        {
            bool result = m_window.MessageLoop();
            if (result == false) // 後から処理を追加しやすいので、改行する
            {
                break;
            }
        }

        // ゲームの更新と描画
        {
            bool result = gameLoop();
            if (result == false)
            {
                break;
            }
        }
    }
}

void Application::Terminate()
{
}

void Application::Finalize() 
{
    m_window.Finalize();
}

bool Application::gameLoop()
{
    // TODO: 更新処理
    // TODO: 描画処理
    // TODO: バックバッファ入れ替え

    Sleep(10);
    return true;
}