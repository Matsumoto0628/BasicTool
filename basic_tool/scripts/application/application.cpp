#include "application.h"
#include "render_context.h"
#include "empty.h"

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
    m_pRenderContext = std::make_shared<RenderContext>(m_window.GetWindowHandle());
    m_pRenderContext->Initialize();

    m_pRenderable = std::make_unique<Empty>(m_pRenderContext);
    m_pRenderable->Initialize();
}

void Application::Start()
{
    m_pRenderContext->Start();
    m_pRenderable->Start();
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
    m_pRenderContext->Terminate();
    m_pRenderable->Terminate();
}

void Application::Finalize() 
{
    m_window.Finalize();
    m_pRenderContext->Finalize();
    m_pRenderable->Finalize();
}

bool Application::gameLoop()
{
    {
        m_pRenderContext->Update();
        m_pRenderable->Update();
    }

    {
        m_pRenderContext->Draw();
        m_pRenderable->Draw();
    }
    
    m_pRenderContext->Swap();

    Sleep(10);
    return true;
}