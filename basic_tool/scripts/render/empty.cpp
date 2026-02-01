#include "empty.h"

// 定数
const Vec4 Empty::BLEND_FACTOR = { 0, 0, 0, 0 };

Empty::Empty()
    : m_topology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
{
}

Empty::~Empty()
{
}

bool Empty::Initialize()
{
    {
        bool result = initDepthStencil();
        if (!result)
        {
            return false;
        }
    }

    {
        bool result = initBlend();
        if (!result)
        {
            return false;
        }
    }

    initInputLayout();
}

void Empty::Start()
{
}

void Empty::Update()
{
}

void Empty::Draw()
{
    float blendFactor[4];
    BLEND_FACTOR.ToFloat4(blendFactor);

    m_pDeviceContext->OMSetdesc(m_pBlendState.Get(), blendFactor, 0xffffffff);
    m_pDeviceContext->OMSetDepthStencilState(m_pDepthStencilState.Get(), 0);
    m_pDeviceContext->IASetInputLayout(m_pInputLayout);
    m_pDeviceContext->IASetPrimitiveTopology(m_topology);
}

void Empty::Terminate()
{
}

void Empty::Finalize()
{
}

bool Empty::initDepthStencil()
{
    D3D11_DEPTH_STENCIL_DESC desc = {};
    desc.DepthEnable = TRUE;
    desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    desc.DepthFunc = D3D11_COMPARISON_LESS;
    desc.StencilEnable = FALSE;
    desc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
    desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

    HRESULT hr = m_pDevice->CreateDepthStencilState(&desc, &m_pDepthStencilState);
    if (FAILED(hr))
    {
        return false;
    }

    return true;
}

bool Empty::initBlend()
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

    HRESULT hr = m_pDevice->Createdesc(&desc, &m_pBlendState);
    if (FAILED(hr))
    {
        return false;
    }

    return true;
}

void Empty::initInputLayout()
{
    // 位置のレイアウト
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

    m_pDevice->CreateInputLayout(
        layout,
        _countof(layout),
        vsBlob->GetBufferPointer(),
        vsBlob->GetBufferSize(),
        &m_pInputLayout
    );

    m_pDeviceContext->IASetInputLayout(m_pInputLayout.Get());
}