#include "application.h"
#include "render_context.h"
#include "scene_manager.h"
#include "game_time.h"
#include "game_input.h"

Application::Application(HINSTANCE hInstance)
    : m_window{ hInstance }
{
}

Application::~Application() 
{
}

void Application::Initialize()
{
    m_window.Initialize();

    // m_windowが初期化してないとコンストラクタが呼び出せない
    m_pContext = std::make_unique<RenderContext>(m_window.GetWindowHandle());
    m_pContext->Initialize();
    m_pSceneManager = std::make_unique<SceneManager>(m_window.GetWindowHandle(), m_pContext.get());
    m_pSceneManager->Initialize();
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

void Application::Finalize() 
{
    m_window.Finalize();
    m_pContext->Finalize();
    m_pSceneManager->Finalize();
}

bool Application::gameLoop()
{
    GameTime::Update();
    GameInput::Update();

    m_pContext->ClearRTV();
    m_pContext->SetRTV();

    m_pContext->Update();
    m_pSceneManager->Update();

    m_pContext->PostEffect();
    m_pContext->Swap();

    Sleep(10);
    return true;
}