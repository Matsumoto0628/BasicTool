#include "square.h"
#include "render_context.h"
#include "camera.h"

// 定数
const Vec4 Square::BLEND_FACTOR = { 0, 0, 0, 0 };

Square::Square(
    const RenderContext* const pContext,
    const Camera* const pCamera,
    const Transform* const pTransform,
    const Vec4& color
)
    : Renderer{ pContext, pCamera, pTransform, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST }, m_color(color) // protectedのメンバ変数は基底クラスで初期化
{
}

Square::~Square()
{
}

void Square::Initialize()
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

    {
        bool result = initIndexBuffer();
        if (!result)
        {
            return;
        }
    }

    {
        bool result = initConstantBufferA();
        if (!result)
        {
            return;
        }
    }
}

void Square::Start()
{
}

void Square::Update()
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
        m_pContext->GetDeviceContext()->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
        m_pContext->GetDeviceContext()->VSSetConstantBuffers(0, 1, m_pConstantBufferA.GetAddressOf());
        m_pContext->GetDeviceContext()->OMSetBlendState(m_pBlendState.Get(), blendFactor, 0xffffffff);
        m_pContext->GetDeviceContext()->OMSetDepthStencilState(m_pDepthStencilState.Get(), 0);
        m_pContext->GetDeviceContext()->RSSetState(m_pRasterizerState.Get());
        m_pContext->GetDeviceContext()->IASetPrimitiveTopology(m_topology);

        m_pContext->GetDeviceContext()->DrawIndexed(6, 0, 0);
    }
}

void Square::Finalize()
{
}

void Square::Show()
{
}

bool Square::initVertexBuffer()
{
    Vertex vertices[4] = {
        { {-1.0f,  1.0f, 0.0f}, m_color }, // 左上
        { {1.0f,  1.0f, 0.0f}, m_color }, // 右上
        { {-1.0f, -1.0f, 0.0f}, m_color }, // 左下
        { {1.0f, -1.0f, 0.0f}, m_color }  // 右下
    };

    D3D11_BUFFER_DESC desc = {};
    desc.ByteWidth = sizeof(vertices);
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    desc.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = vertices;

    HRESULT hr = m_pContext->GetDevice()->CreateBuffer(
        &desc,
        &initData,
        &m_pVertexBuffer
    );

    return SUCCEEDED(hr);
}


bool Square::initIndexBuffer()
{
    unsigned int indices[6] = {
        0, 1, 2, // 左上三角形
        2, 1, 3  // 右下三角形
    };

    D3D11_BUFFER_DESC desc = {};
    desc.ByteWidth = sizeof(indices);
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = indices;

    HRESULT hr = m_pContext->GetDevice()->CreateBuffer(
        &desc,
        &initData,
        &m_pIndexBuffer
    );

    return SUCCEEDED(hr);
}

bool Square::initConstantBufferA()
{
    D3D11_BUFFER_DESC desc = {};
    desc.ByteWidth = sizeof(ConstantBufferA);
    desc.Usage = D3D11_USAGE_DYNAMIC; // Map/Unmapで書き換える
    desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    HRESULT hr = m_pContext->GetDevice()->CreateBuffer(
        &desc,
        nullptr,
        &m_pConstantBufferA
    );
    if (FAILED(hr))
    {
        return false;
    }

    return true;
}

void Square::updateConstantBufferA()
{
    // 渡すもの
    ConstantBufferA cb;
    m_pTransform->GetMatrix().Transpose().ToFloat4x4(cb.world);
    m_pCamera->GetView().Transpose().ToFloat4x4(cb.view);
    m_pCamera->GetProj().Transpose().ToFloat4x4(cb.proj);

    D3D11_MAPPED_SUBRESOURCE mapped = {};
    if (SUCCEEDED(m_pContext->GetDeviceContext()->Map(m_pConstantBufferA.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped)))
    {
        memcpy(mapped.pData, &cb, sizeof(ConstantBufferA));
        m_pContext->GetDeviceContext()->Unmap(m_pConstantBufferA.Get(), 0);
    }
}

bool Square::initVertexShader()
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

bool Square::initInputLayout(ID3DBlob* vsBlob)
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

bool Square::initPixelShader()
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