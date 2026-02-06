#include "application.h"
#include "render_context.h"
#include "gui.h"
#include "test_scene.h"

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

    // m_windowが初期化してないとコンストラクタが呼び出せない
    {
        m_pContext = std::make_unique<RenderContext>(m_window.GetWindowHandle());
        m_pContext->Initialize();
        m_pScene = std::make_unique<TestScene>(m_pContext.get());
        m_pScene->Initialize();
        m_pGui = std::make_unique<Gui>(m_window.GetWindowHandle(), m_pContext.get());
        m_pGui->Initialize();
    }
}

void Application::Start()
{
    m_pContext->Start();
    m_pScene->Start();
    m_pGui->Start();
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
    m_pContext->Terminate();
    m_pScene->Terminate();
    m_pGui->Terminate();
}

void Application::Finalize() 
{
    m_window.Finalize();
    m_pContext->Finalize();
    m_pScene->Finalize();
    m_pGui->Finalize();
}

bool Application::gameLoop()
{
    m_pContext->Update();
    m_pScene->Update();
    m_pGui->Update();
    
    m_pContext->Swap();

    Sleep(10);
    return true;
}