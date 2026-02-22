#include "render_context.h"

// 定数
const Vec4 RenderContext::BACK_BUFFER_COLOR = { 0.2f, 0.2f, 0.2f, 1.0f };

RenderContext::RenderContext(HWND hWnd)
    : m_hWnd{ hWnd }
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

    {
        bool result = initHDR();
        if (!result)
        {
            return false;
        }
    }

    {
        bool result = initBloom();
        if (!result)
        {
            return false;
        }
    }

    {
        bool result = initConstantBufferBlur();
        if (!result)
        {
            return false;
        }
    }

    {
        bool result = initVertexShader();
        if (!result)
        {
            return false;
        }
    }

    {
        bool result = initPixelShader();
        if (!result)
        {
            return false;
        }
    }

    {
        bool result = initSampler();
        if (!result)
        {
            return false;
        }
    }
    
    initViewPort();

    return true;
}

void RenderContext::Update() 
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

void RenderContext::ClearRTV()
{
    float color[4];
    BACK_BUFFER_COLOR.ToFloat4(color);

    m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView.Get(), color);
    m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetViewHDR.Get(), color);
    m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetViewBloomA.Get(), color);
    m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetViewBloomB.Get(), color);
    m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void RenderContext::SetRTV()
{
    m_pDeviceContext->OMSetRenderTargets(1, m_pRenderTargetViewHDR.GetAddressOf(), m_pDepthStencilView.Get());
}

void RenderContext::PostEffect()
{
    m_pDeviceContext->PSSetSamplers(0, 1, m_pSamplerLinear.GetAddressOf());

    ID3D11RenderTargetView* nullRTV[1] = { nullptr };
    ID3D11ShaderResourceView* nullSRV[2] = { nullptr, nullptr };
    m_pDeviceContext->OMSetRenderTargets(1, nullRTV, nullptr);
    m_pDeviceContext->PSSetShaderResources(0, 1, nullSRV);

    // 明るさ抽出
    {
        m_pDeviceContext->RSSetViewports(1, &m_viewPortBloom[0]);
        m_pDeviceContext->OMSetRenderTargets(1, m_pRenderTargetViewBloomA.GetAddressOf(), nullptr); // ping-pongするのでAだけ
        m_pDeviceContext->PSSetShader(m_pPixelShaderExtract.Get(), nullptr, 0);
        m_pDeviceContext->PSSetShaderResources(0, 1, m_pShaderResourceViewHDR.GetAddressOf());
        drawFullscreen();

        m_pDeviceContext->OMSetRenderTargets(1, nullRTV, nullptr);
        m_pDeviceContext->PSSetShaderResources(0, 1, nullSRV);
    }

    // ブラーA
    {
        m_pDeviceContext->RSSetViewports(1, &m_viewPortBloom[0]);
        m_pDeviceContext->OMSetRenderTargets(1, m_pRenderTargetViewBloomB.GetAddressOf(), nullptr);
        m_pDeviceContext->PSSetShader(m_pPixelShaderBlur.Get(), nullptr, 0);
        m_pDeviceContext->PSSetShaderResources(0, 1, m_pShaderResourceViewBloomA.GetAddressOf());
        updateConstantBufferBlur({ 1.0f, 0 });
        m_pDeviceContext->PSSetConstantBuffers(0, 1, m_pConstantBufferBlur.GetAddressOf());
        drawFullscreen();

        m_pDeviceContext->OMSetRenderTargets(1, nullRTV, nullptr);
        m_pDeviceContext->PSSetShaderResources(0, 1, nullSRV);
    }

    // ブラーB
    {
        m_pDeviceContext->RSSetViewports(1, &m_viewPortBloom[0]);
        m_pDeviceContext->OMSetRenderTargets(1, m_pRenderTargetViewBloomA.GetAddressOf(), nullptr);
        m_pDeviceContext->PSSetShader(m_pPixelShaderBlur.Get(), nullptr, 0);
        m_pDeviceContext->PSSetShaderResources(0, 1, m_pShaderResourceViewBloomB.GetAddressOf());
        updateConstantBufferBlur({ 0, 1.0f });
        m_pDeviceContext->PSSetConstantBuffers(0, 1, m_pConstantBufferBlur.GetAddressOf());
        drawFullscreen();

        m_pDeviceContext->OMSetRenderTargets(1, nullRTV, nullptr);
        m_pDeviceContext->PSSetShaderResources(0, 1, nullSRV);
    }

    // 合成
    {
        m_pDeviceContext->RSSetViewports(1, &m_viewPort[0]);
        m_pDeviceContext->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), nullptr);
        m_pDeviceContext->PSSetShader(m_pPixelShaderComposite.Get(), nullptr, 0);
        ID3D11ShaderResourceView* srvs[2] = { m_pShaderResourceViewHDR.Get(), m_pShaderResourceViewBloomA.Get()};
        m_pDeviceContext->PSSetShaderResources(0, 2, srvs);
        drawFullscreen();

        m_pDeviceContext->OMSetRenderTargets(1, nullRTV, nullptr);
        m_pDeviceContext->PSSetShaderResources(0, 1, nullSRV);
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
    {
        m_viewPort[0].TopLeftX = 0.0f;
        m_viewPort[0].TopLeftY = 0.0f;
        m_viewPort[0].Width = static_cast<float>(m_screenWidth);
        m_viewPort[0].Height = static_cast<float>(m_screenHeight);
        m_viewPort[0].MinDepth = 0.0f;
        m_viewPort[0].MaxDepth = 1.0f;
    }

    {
        m_viewPortBloom[0].TopLeftX = 0.0f;
        m_viewPortBloom[0].TopLeftY = 0.0f;
        m_viewPortBloom[0].Width = m_screenWidth * 0.5f;
        m_viewPortBloom[0].Height = m_screenHeight * 0.5f;
        m_viewPortBloom[0].MinDepth = 0.0f;
        m_viewPortBloom[0].MaxDepth = 1.0f;
    }
}

bool RenderContext::initHDR()
{
    D3D11_TEXTURE2D_DESC desc = {};
    desc.Width = m_screenWidth;
    desc.Height = m_screenHeight;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT; // HDR用のフォーマット
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

    Microsoft::WRL::ComPtr<ID3D11Texture2D> hdrTexture = nullptr;

    // テクスチャを作成して
    {
        HRESULT hr = m_pDevice->CreateTexture2D(&desc, nullptr, &hdrTexture);
        if (FAILED(hr))
        {
            return false;
        }
    }

    // RTVを作成
    {
        HRESULT hr = m_pDevice->CreateRenderTargetView(
            hdrTexture.Get(),
            nullptr,
            &m_pRenderTargetViewHDR
        );
        if (FAILED(hr))
        {
            return false;
        }
    }

    // SRVを作成
    {
        HRESULT hr = m_pDevice->CreateShaderResourceView(
            hdrTexture.Get(),
            nullptr,
            &m_pShaderResourceViewHDR
        );
        if (FAILED(hr))
        {
            return false;
        }
    }

    hdrTexture = nullptr;
    
    return true;
}

bool RenderContext::initBloom()
{
    D3D11_TEXTURE2D_DESC desc = {};
    desc.Width = m_screenWidth / 2;
    desc.Height = m_screenHeight / 2;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT; // HDR用のフォーマット
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

    Microsoft::WRL::ComPtr<ID3D11Texture2D> bloomTextureA = nullptr;
    Microsoft::WRL::ComPtr<ID3D11Texture2D> bloomTextureB = nullptr;

    // テクスチャを作成して
    {
        HRESULT hr = m_pDevice->CreateTexture2D(&desc, nullptr, &bloomTextureA);
        if (FAILED(hr))
        {
            return false;
        }

        hr = m_pDevice->CreateTexture2D(&desc, nullptr, &bloomTextureB);
        if (FAILED(hr))
        {
            return false;
        }
    }

    // RTVを作成
    {
        HRESULT hr = m_pDevice->CreateRenderTargetView(
            bloomTextureA.Get(),
            nullptr,
            &m_pRenderTargetViewBloomA
        );
        if (FAILED(hr))
        {
            return false;
        }

        hr = m_pDevice->CreateRenderTargetView(
            bloomTextureB.Get(),
            nullptr,
            &m_pRenderTargetViewBloomB
        );
        if (FAILED(hr))
        {
            return false;
        }
    }

    // SRVを作成
    {
        HRESULT hr = m_pDevice->CreateShaderResourceView(
            bloomTextureA.Get(),
            nullptr,
            &m_pShaderResourceViewBloomA
        );
        if (FAILED(hr))
        {
            return false;
        }

        hr = m_pDevice->CreateShaderResourceView(
            bloomTextureB.Get(),
            nullptr,
            &m_pShaderResourceViewBloomB
        );
        if (FAILED(hr))
        {
            return false;
        }
    }

    bloomTextureA = nullptr;
    bloomTextureB = nullptr;

    return true;
}

bool RenderContext::initConstantBufferBlur()
{
    D3D11_BUFFER_DESC desc = {};
    desc.ByteWidth = sizeof(ConstantBufferBlur);
    desc.Usage = D3D11_USAGE_DYNAMIC;
    desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    return SUCCEEDED(m_pDevice->CreateBuffer(&desc, nullptr, &m_pConstantBufferBlur));
}

bool RenderContext::initVertexShader()
{
    Microsoft::WRL::ComPtr<ID3DBlob> vsBlob;

    HRESULT hr = D3DReadFileToBlob(L"scripts/shader/fullscreen_vs.cso", &vsBlob);
    if (FAILED(hr))
    {
        return false;
    }

    hr = m_pDevice->CreateVertexShader(
        vsBlob->GetBufferPointer(),
        vsBlob->GetBufferSize(),
        nullptr,
        &m_pVertexShaderFullscreen
    );
    if (FAILED(hr))
    {
        return false;
    }

    vsBlob = nullptr;

    return true;
}

bool RenderContext::initPixelShader()
{
    Microsoft::WRL::ComPtr<ID3DBlob> psBlob;

    // extract
    {
        HRESULT hr = D3DReadFileToBlob(L"scripts/shader/extract_pos_uv_ps.cso", &psBlob);
        if (FAILED(hr))
        {
            return false;
        }

        hr = m_pDevice->CreatePixelShader(
            psBlob->GetBufferPointer(),
            psBlob->GetBufferSize(),
            nullptr,
            &m_pPixelShaderExtract
        );
        if (FAILED(hr))
        {
            return false;
        }
    }

    // blur
    {
        HRESULT hr = D3DReadFileToBlob(L"scripts/shader/blur_pos_uv_ps.cso", &psBlob);
        if (FAILED(hr))
        {
            return false;
        }

        hr = m_pDevice->CreatePixelShader(
            psBlob->GetBufferPointer(),
            psBlob->GetBufferSize(),
            nullptr,
            &m_pPixelShaderBlur
        );
        if (FAILED(hr))
        {
            return false;
        }
    }

    // composite
    {
        HRESULT hr = D3DReadFileToBlob(L"scripts/shader/composite_pos_uv_ps.cso", &psBlob);
        if (FAILED(hr))
        {
            return false;
        }

        hr = m_pDevice->CreatePixelShader(
            psBlob->GetBufferPointer(),
            psBlob->GetBufferSize(),
            nullptr,
            &m_pPixelShaderComposite
        );
        if (FAILED(hr))
        {
            return false;
        }
    }

    psBlob = nullptr;

    return true;
}

bool RenderContext::initSampler()
{
    D3D11_SAMPLER_DESC desc = {};
    desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    desc.MinLOD = 0;
    desc.MaxLOD = D3D11_FLOAT32_MAX;

    return SUCCEEDED(m_pDevice->CreateSamplerState(&desc, &m_pSamplerLinear));
}

void RenderContext::updateConstantBufferBlur(const Vec2& dir)
{
    ConstantBufferBlur cb;
    Vec2 vTexelSize = { 1.0f / (m_screenWidth * 0.5f), 1.0f / (m_screenHeight * 0.5f) };
    vTexelSize.ToFloat2(cb.texelSize);
    dir.ToFloat2(cb.dir);

    D3D11_MAPPED_SUBRESOURCE mapped = {};
    if (SUCCEEDED(m_pDeviceContext->Map(m_pConstantBufferBlur.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped)))
    {
        memcpy(mapped.pData, &cb, sizeof(ConstantBufferBlur));
        m_pDeviceContext->Unmap(m_pConstantBufferBlur.Get(), 0);
    }
}

void RenderContext::drawFullscreen()
{
    m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    m_pDeviceContext->IASetInputLayout(nullptr);
    m_pDeviceContext->VSSetShader(m_pVertexShaderFullscreen.Get(), nullptr, 0);

    m_pDeviceContext->Draw(3, 0);
}