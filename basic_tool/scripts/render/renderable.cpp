#include "renderable.h"
#include "render_context.h"
#include "camera.h"

Renderable::Renderable(RenderContext* pContext, Camera* pCamera)
    : m_pContext(pContext), 
    m_pCamera(pCamera),
    m_topology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
{
}

Renderable::Renderable(RenderContext* pContext, Camera* pCamera, D3D11_PRIMITIVE_TOPOLOGY topology)
    : m_pContext(pContext),
    m_pCamera(pCamera),
    m_topology(topology)
{
}

bool Renderable::initDepthStencil()
{
    D3D11_DEPTH_STENCIL_DESC desc = {};
    desc.DepthEnable = TRUE;
    desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    desc.DepthFunc = D3D11_COMPARISON_LESS;
    desc.StencilEnable = FALSE;
    desc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
    desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

    HRESULT hr = m_pContext->GetDevice()->CreateDepthStencilState(&desc, &m_pDepthStencilState);
    if (FAILED(hr))
    {
        return false;
    }

    return true;
}

bool Renderable::initBlend()
{
    D3D11_BLEND_DESC desc = {};
    desc.AlphaToCoverageEnable = FALSE;
    desc.IndependentBlendEnable = FALSE;
    desc.RenderTarget[0].BlendEnable = TRUE;
    desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    HRESULT hr = m_pContext->GetDevice()->CreateBlendState(&desc, &m_pBlendState);
    if (FAILED(hr))
    {
        return false;
    }

    return true;
}

bool Renderable::initVertexShader() 
{
    Microsoft::WRL::ComPtr<ID3DBlob> vsBlob;

    HRESULT hr = D3DReadFileToBlob(L"scripts/shader/test_vs.cso", &vsBlob);
    if (FAILED(hr)) 
    {
        return false;
    }

    hr = m_pContext->GetDevice()->CreateVertexShader(
        vsBlob->GetBufferPointer(),
        vsBlob->GetBufferSize(),
        nullptr,
        &m_pVertexShader
    );
    if (FAILED(hr))
    {
        return false;
    }

    // vsBlobが必要なのでここでInputLayout初期化
    {
        bool result = initInputLayout(vsBlob.Get());
        if (!result) 
        {
            return false;
        }
    }

    vsBlob = nullptr;

    return true;
}

bool Renderable::initInputLayout(ID3DBlob* vsBlob)
{
    D3D11_INPUT_ELEMENT_DESC position = {
        "POSITION",
        0,
        DXGI_FORMAT_R32G32B32_FLOAT,
        0,
        0,
        D3D11_INPUT_PER_VERTEX_DATA,
        0
    };

    D3D11_INPUT_ELEMENT_DESC layout[] = {
        position
    };

    HRESULT hr = m_pContext->GetDevice()->CreateInputLayout(
        layout,
        _countof(layout),
        vsBlob->GetBufferPointer(),
        vsBlob->GetBufferSize(),
        &m_pInputLayout
    );
    if (FAILED(hr))
    {
        return false;
    }

    return true;
}

bool Renderable::initPixelShader()
{
    Microsoft::WRL::ComPtr<ID3DBlob> psBlob;

    HRESULT hr = D3DReadFileToBlob(L"scripts/shader/test_ps.cso", &psBlob);
    if (FAILED(hr))
    {
        return false;
    }

    hr = m_pContext->GetDevice()->CreatePixelShader(
        psBlob->GetBufferPointer(),
        psBlob->GetBufferSize(),
        nullptr,
        &m_pPixelShader
    );
    if (FAILED(hr))
    {
        return false;
    }

    psBlob = nullptr;

    return true;
}