#include "application.h"
#include "render_context.h"
#include "sphere.h"
#include "gui.h"

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

    // m_pRenderContextが初期化していないとコンストラクタが呼び出せない
    m_pRenderable = std::make_unique<Sphere>(m_pRenderContext);
    m_pRenderable->Initialize();
    m_pGui = std::make_unique<Gui>(m_window.GetWindowHandle(), m_pRenderContext);
    m_pGui->Initialize();
}

void Application::Start()
{
    m_pRenderContext->Start();
    m_pRenderable->Start();
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
    m_pRenderContext->Terminate();
    m_pRenderable->Terminate();
    m_pGui->Terminate();
}

void Application::Finalize() 
{
    m_window.Finalize();
    m_pRenderContext->Finalize();
    m_pRenderable->Finalize();
    m_pGui->Finalize();
}

bool Application::gameLoop()
{
    {
        m_pRenderContext->Update();
        m_pRenderable->Update();
        m_pGui->Update();
    }

    {
        m_pRenderContext->Draw();
        m_pRenderable->Draw();
        m_pGui->Draw();
    }
    
    m_pRenderContext->Swap();

    Sleep(10);
    return true;
}