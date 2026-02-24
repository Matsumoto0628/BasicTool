#include "line.h"
#include "render_context.h"
#include <vector>
#include "camera.h"

const Vec4 Line::BLEND_FACTOR = { 0,0,0,0 };

Line::Line(const RenderContext* const pContext, const Camera* const pCamera, const Vec4& color)
    : Renderer{ Type::Line, pContext, pCamera, nullptr, D3D_PRIMITIVE_TOPOLOGY_LINELIST}, m_color{color}
{
}

Line::~Line()
{
}

void Line::Initialize()
{
    {
        bool result = initDepthStencil();
        if (!result)
        {
            return;
        }
    }

    {
        bool result = initBlend();
        if (!result)
        {
            return;
        }
    }

    {
        bool result = initRasterizer();
        if (!result)
        {
            return;
        }
    }

    // ここでInputLayoutも初期化
    {
        bool result = initVertexShader();
        if (!result)
        {
            return;
        }
    }

    {
        bool result = initPixelShader();
        if (!result)
        {
            return;
        }
    }

    {
        bool result = initVertexBuffer();
        if (!result)
        {
            return;
        }
    }

    // initIndexBufferはいらない

    {
        bool result = initConstantBufferA();
        if (!result)
        {
            return;
        }
    }
}

void Line::Start()
{
}

void Line::Update()
{
    // 更新
    {
        updateConstantBufferA();
    }

    // 描画
    {
        float blendFactor[4];
        BLEND_FACTOR.ToFloat4(blendFactor);

        m_pContext->GetDeviceContext()->VSSetShader(m_pVertexShader.Get(), nullptr, 0);
        m_pContext->GetDeviceContext()->IASetInputLayout(m_pInputLayout.Get());
        m_pContext->GetDeviceContext()->PSSetShader(m_pPixelShader.Get(), nullptr, 0);
        m_pContext->GetDeviceContext()->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &STRIDE, &OFFSET); 
        // SetIndexBufferはいらない
        m_pContext->GetDeviceContext()->VSSetConstantBuffers(0, 1, m_pConstantBufferA.GetAddressOf());
        m_pContext->GetDeviceContext()->OMSetBlendState(m_pBlendState.Get(), blendFactor, 0xffffffff);
        m_pContext->GetDeviceContext()->OMSetDepthStencilState(m_pDepthStencilState.Get(), 0);
        m_pContext->GetDeviceContext()->RSSetState(m_pRasterizerState.Get());
        m_pContext->GetDeviceContext()->IASetPrimitiveTopology(m_topology);

        m_pContext->GetDeviceContext()->Draw(VERTEX_COUNT, 0);
    }
}

void Line::Finalize()
{
}

void Line::Show()
{
}

bool Line::initVertexBuffer()
{
    Vertex vertices[VERTEX_COUNT] = {
        {{0,0,0}, m_color},
        {{1,0,0}, m_color},
    };

    D3D11_BUFFER_DESC desc = {};
    desc.ByteWidth = sizeof(vertices);
    desc.Usage = D3D11_USAGE_DYNAMIC;
    desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; //mapする

    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = vertices;

    return SUCCEEDED(m_pContext->GetDevice()->CreateBuffer(&desc, &initData, &m_pVertexBuffer));
}

bool Line::initIndexBuffer()
{
    return true;
}

bool Line::initConstantBufferA()
{
    D3D11_BUFFER_DESC desc = {};
    desc.ByteWidth = sizeof(ConstantBufferA);
    desc.Usage = D3D11_USAGE_DYNAMIC;
    desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    return SUCCEEDED(m_pContext->GetDevice()->CreateBuffer(&desc, nullptr, &m_pConstantBufferA));
}

void Line::updateConstantBufferA()
{
    ConstantBufferA cb;
    Mat4x4::Identity().ToFloat4x4(cb.world);
    m_pCamera->GetView().Transpose().ToFloat4x4(cb.view);
    m_pCamera->GetProj().Transpose().ToFloat4x4(cb.proj);

    D3D11_MAPPED_SUBRESOURCE mapped = {};
    if (SUCCEEDED(m_pContext->GetDeviceContext()->Map(m_pConstantBufferA.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped)))
    {
        memcpy(mapped.pData, &cb, sizeof(ConstantBufferA));
        m_pContext->GetDeviceContext()->Unmap(m_pConstantBufferA.Get(), 0);
    }
}

Json Line::Serialize() const
{
    return {
		{ "color", m_color }
    };
}

void Line::Deserialize(const Json& j)
{
}

bool Line::initVertexShader()
{
    Microsoft::WRL::ComPtr<ID3DBlob> vsBlob;

    HRESULT hr = D3DReadFileToBlob(L"scripts/shader/test_pos_color_vs.cso", &vsBlob);
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

bool Line::initInputLayout(ID3DBlob* vsBlob)
{
    D3D11_INPUT_ELEMENT_DESC positionDesc = {
        "POSITION",
        0,
        DXGI_FORMAT_R32G32B32_FLOAT,
        0,
        offsetof(Vertex, pos), // Vec3は12ではなく16(XMVECTORだから)
        D3D11_INPUT_PER_VERTEX_DATA,
        0
    };

    D3D11_INPUT_ELEMENT_DESC colorDesc = {
        "TEXCOORD",
        0,
        DXGI_FORMAT_R32G32B32A32_FLOAT,
        0,
        offsetof(Vertex, color),
        D3D11_INPUT_PER_VERTEX_DATA,
        0
    };

    D3D11_INPUT_ELEMENT_DESC layout[] = {
        positionDesc,
        colorDesc
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

bool Line::initPixelShader()
{
    Microsoft::WRL::ComPtr<ID3DBlob> psBlob;

    HRESULT hr = D3DReadFileToBlob(L"scripts/shader/test_pos_color_ps.cso", &psBlob);
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

bool Line::initDepthStencil()
{
    D3D11_DEPTH_STENCIL_DESC desc = {};
    desc.DepthEnable = FALSE; // 常に前面に表示
    desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
    desc.DepthFunc = D3D11_COMPARISON_ALWAYS;
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

void Line::SetLine(const Vec3& start, const Vec3& end) 
{
    Vertex vertices[2];
    vertices[0] = { start, m_color };
    vertices[1] = { end,   m_color };

    D3D11_MAPPED_SUBRESOURCE mapped{};
    m_pContext->GetDeviceContext()->Map(
        m_pVertexBuffer.Get(),
        0,
        D3D11_MAP_WRITE_DISCARD,
        0,
        &mapped
    );

    memcpy(mapped.pData, vertices, sizeof(vertices));

    m_pContext->GetDeviceContext()->Unmap(m_pVertexBuffer.Get(), 0);
}