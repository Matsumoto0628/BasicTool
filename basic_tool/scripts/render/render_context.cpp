#include "render_context.h"

// 定数
const Vec4 RenderContext::BACK_BUFFER_COLOR = { 0, 0, 1.0f, 1.0f };

RenderContext::RenderContext(HWND hWnd)
    : m_hWnd(hWnd), m_nearClipDist(0.1f), m_farClipDist(1000.0f), m_fov(DegToRad(30.0f))
{
    m_pFeatureLevels[0] = D3D_FEATURE_LEVEL_11_1;
    m_pFeatureLevels[1] = D3D_FEATURE_LEVEL_11_0;
}

RenderContext::~RenderContext()
{
    m_hWnd = nullptr;
}

bool RenderContext::Initialize()
{
    // ウィンドウに合わせてスクリーンサイズ初期化
    {
        RECT rc;
        GetClientRect(m_hWnd, &rc);
        m_screenWidth = rc.right - rc.left;
        m_screenHeight = rc.bottom - rc.top;
    }
    
    {
        bool result = initDeviceAndSwapChain();
        if (!result)
        {
            return false;
        }
    }
    
    {
        bool result = initBackBuffer();
        if (!result)
        {
            return false;
        }
    }

    {
        bool result = initDepthStencilView();
        if (!result)
        {
            return false;
        }
    }
    
    initViewPort();

    return true;
}

void RenderContext::Start() 
{
}

void RenderContext::Update() 
{
}

void RenderContext::Draw()
{
    if (!m_pDeviceContext || !m_pRenderTargetView || !m_pDepthStencilView)
    {
        return;
    }

    float color[4];
    BACK_BUFFER_COLOR.ToFloat4(color);

    // 全てのオブジェクト共通で行う処理
    m_pDeviceContext->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), m_pDepthStencilView.Get());
    m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView.Get(), color);
    m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    m_pDeviceContext->RSSetViewports(1, &m_viewPort[0]);
}

void RenderContext::Terminate()
{
}

void RenderContext::Finalize() 
{
    if (m_pDeviceContext)
    {
        m_pDeviceContext->ClearState();
    }

    // スワップチェインをウインドウにする
    if (m_pSwapChain)
    {
        m_pSwapChain->SetFullscreenState(FALSE, nullptr);
    }
}

void RenderContext::Swap()
{
    m_pSwapChain->Present(0, 0);
}

bool RenderContext::initBackBuffer()
{
    Microsoft::WRL::ComPtr<ID3D11Texture2D> pBackBuffer;

    // スワップチェインから最初のバックバッファを取得
    {
        HRESULT hr = m_pSwapChain->GetBuffer(
            0,
            __uuidof(ID3D11Texture2D),
            reinterpret_cast<void**>(pBackBuffer.GetAddressOf())
        );
        if (FAILED(hr))
        {
            return false;
        }
    }

    // バックバッファの描画ターゲットビューを作成
    {
        HRESULT hr = m_pDevice->CreateRenderTargetView(
            pBackBuffer.Get(),
            nullptr,
            &m_pRenderTargetView
        );
        if (FAILED(hr))
        {
            return false;
        }
    }

    pBackBuffer = nullptr;

    return true;
}

bool RenderContext::initDeviceAndSwapChain()
{
    // スワップチェインの設定
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = BACK_BUFFER_NUM;
    sd.BufferDesc.Width = m_screenWidth;
    sd.BufferDesc.Height = m_screenHeight;
    sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;    // リフレッシュレート（分母）
    sd.BufferDesc.RefreshRate.Denominator = 1;    // リフレッシュレート（分子）
    sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE;
    sd.BufferDesc.Scaling = DXGI_MODE_SCALING_CENTERED;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = m_hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

#if 1 // デバッグ状態だと画像が表示できないので苦肉の策
    UINT createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT; // RGBAだと画像が表示できない
#elif defined(DEBUG) || defined(_DEBUG)
    UINT createDeviceFlags = D3D11_CREATE_DEVICE_DEBUG; // 同様に画像が表示できない
#else
    UINT createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#endif

    const D3D_DRIVER_TYPE DriverTypes[] = {
        D3D_DRIVER_TYPE_HARDWARE,
        D3D_DRIVER_TYPE_WARP,
        D3D_DRIVER_TYPE_REFERENCE,
    };

    // デバイスを作成
    HRESULT hr;
    for (auto type : DriverTypes)
    {
        hr = D3D11CreateDeviceAndSwapChain(
            nullptr,
            type,
            nullptr,
            createDeviceFlags,
            m_pFeatureLevels,
            FEATURE_LEVELS_NUM,
            D3D11_SDK_VERSION,
            &sd,
            &m_pSwapChain,
            &m_pDevice,
            &m_featureLevelsSupported,
            &m_pDeviceContext
        );

        if (SUCCEEDED(hr))
        {
            break;
        }
    }
    if (FAILED(hr))
    {
        return false;
    }

    return true;
}

bool RenderContext::initDepthStencilView()
{
    // 深度ステンシル用のテクスチャの設定
    D3D11_TEXTURE2D_DESC depthDesc = {};
    depthDesc.Width = m_screenWidth;
    depthDesc.Height = m_screenHeight;
    depthDesc.MipLevels = 1;
    depthDesc.ArraySize = 1;
    depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthDesc.SampleDesc.Count = 1;
    depthDesc.Usage = D3D11_USAGE_DEFAULT;
    depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

    Microsoft::WRL::ComPtr<ID3D11Texture2D> depthTexture = nullptr;

    // テクスチャを作成して
    {
        HRESULT hr = m_pDevice->CreateTexture2D(&depthDesc, nullptr, &depthTexture);
        if (FAILED(hr))
        {
            return false;
        }
    }

    // 深度ステンシルビューを作成
    {
        HRESULT hr = m_pDevice->CreateDepthStencilView(
            depthTexture.Get(),
            nullptr,
            &m_pDepthStencilView
        );
        if (FAILED(hr))
        {
            return false;
        }
    }

    depthTexture = nullptr;

    return true;
}

void RenderContext::initViewPort() 
{
    m_viewPort[0].TopLeftX = 0.0f;
    m_viewPort[0].TopLeftY = 0.0f;
    m_viewPort[0].Width = static_cast<float>(m_screenWidth);
    m_viewPort[0].Height = static_cast<float>(m_screenHeight);
    m_viewPort[0].MinDepth = 0.0f;
    m_viewPort[0].MaxDepth = 1.0f;
}