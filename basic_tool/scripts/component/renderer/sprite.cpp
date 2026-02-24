#include "sprite.h"
#include "render_context.h"
#include "camera.h"
#include "imgui.h"

// 定数
const Vec4 Sprite::BLEND_FACTOR = { 0, 0, 0, 0 };

Sprite::Sprite(
    const RenderContext* const pContext,
    const Camera* const pCamera,
    const Transform* const pTransform,
    const Vec4& color
)
    : Renderer{ Type::Sprite, pContext, pCamera, pTransform, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST}, m_color(color) // protectedのメンバ変数は基底クラスで初期化
{
}

Sprite::~Sprite()
{
}

void Sprite::Initialize()
{
    {
        bool result = initDepthStencil();
        if (!result)
        {
            return;
        }
    }

    {
        bool result = initDepthStencilNoMask();
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
        bool result = initBlendNoColor();
        if (!result)
        {
            return;
        }
    }

    {
        bool result = initSampler();
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
        bool result = initTexture();
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
        bool result = initPixelShaderZOnly();
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

void Sprite::Start()
{
}

void Sprite::Update()
{
    // 更新
    {
        updateConstantBufferA();
    }

    // 描画
    {
        float blendFactor[4];
        BLEND_FACTOR.ToFloat4(blendFactor);

        // Zのみ
        {
            m_pContext->GetDeviceContext()->VSSetShader(m_pVertexShader.Get(), nullptr, 0);
            m_pContext->GetDeviceContext()->IASetInputLayout(m_pInputLayout.Get());
            m_pContext->GetDeviceContext()->PSSetShaderResources(0, 1, m_pTexture.GetAddressOf()); // テクスチャ用
            m_pContext->GetDeviceContext()->PSSetShader(m_pPixelShaderZOnly.Get(), nullptr, 0); // 色なし
            m_pContext->GetDeviceContext()->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &STRIDE, &OFFSET);
            m_pContext->GetDeviceContext()->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
            m_pContext->GetDeviceContext()->VSSetConstantBuffers(0, 1, m_pConstantBufferA.GetAddressOf());
            m_pContext->GetDeviceContext()->OMSetBlendState(m_pBlendNoColorState.Get(), blendFactor, 0xffffffff); // 色なし
            m_pContext->GetDeviceContext()->OMSetDepthStencilState(m_pDepthStencilState.Get(), 0);
            m_pContext->GetDeviceContext()->PSSetSamplers(0, 1, m_pSamplerState.GetAddressOf()); // テクスチャ用
            m_pContext->GetDeviceContext()->RSSetState(m_pRasterizerState.Get());
            m_pContext->GetDeviceContext()->IASetPrimitiveTopology(m_topology);

            m_pContext->GetDeviceContext()->DrawIndexed(6, 0, 0);
        }

        // 通常
        {
            m_pContext->GetDeviceContext()->VSSetShader(m_pVertexShader.Get(), nullptr, 0);
            m_pContext->GetDeviceContext()->IASetInputLayout(m_pInputLayout.Get());
            m_pContext->GetDeviceContext()->PSSetShaderResources(0, 1, m_pTexture.GetAddressOf()); // テクスチャ用
            m_pContext->GetDeviceContext()->PSSetShader(m_pPixelShader.Get(), nullptr, 0);
            m_pContext->GetDeviceContext()->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &STRIDE, &OFFSET);
            m_pContext->GetDeviceContext()->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
            m_pContext->GetDeviceContext()->VSSetConstantBuffers(0, 1, m_pConstantBufferA.GetAddressOf());
            m_pContext->GetDeviceContext()->OMSetBlendState(m_pBlendState.Get(), blendFactor, 0xffffffff);
            m_pContext->GetDeviceContext()->OMSetDepthStencilState(m_pDepthStencilNoMaskState.Get(), 0); // Zなし
            m_pContext->GetDeviceContext()->PSSetSamplers(0, 1, m_pSamplerState.GetAddressOf()); // テクスチャ用
            m_pContext->GetDeviceContext()->RSSetState(m_pRasterizerState.Get());
            m_pContext->GetDeviceContext()->IASetPrimitiveTopology(m_topology);

            m_pContext->GetDeviceContext()->DrawIndexed(6, 0, 0);
        }
    }
}

void Sprite::Finalize()
{
}

void Sprite::Show()
{
    ImGui::PushID(this);
    if (ImGui::CollapsingHeader("Sprite Renderer"))
    {
    }
    ImGui::PopID();
}

bool Sprite::initRasterizer()
{
    D3D11_RASTERIZER_DESC desc = {};
    desc.FillMode = D3D11_FILL_SOLID;
    desc.CullMode = D3D11_CULL_NONE;   // 両面描画
    desc.FrontCounterClockwise = FALSE;
    desc.DepthClipEnable = TRUE;

    HRESULT hr = m_pContext->GetDevice()->CreateRasterizerState(
        &desc,
        &m_pRasterizerState
    );

    return SUCCEEDED(hr);
}

bool Sprite::initVertexBuffer()
{
    Vertex vertices[4] = {
        { {-1.0f,  1.0f, 0.0f}, m_color, {0,0} }, // 左上
        { {1.0f,  1.0f, 0.0f}, m_color, {1,0} }, // 右上
        { {-1.0f, -1.0f, 0.0f}, m_color, {0,1} }, // 左下
        { {1.0f, -1.0f, 0.0f}, m_color, {1,1} }  // 右下
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


bool Sprite::initIndexBuffer()
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

bool Sprite::initConstantBufferA()
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

void Sprite::updateConstantBufferA()
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

Json Sprite::Serialize() const
{
    return Json();
}

void Sprite::Deserialize(const Json& j)
{
}

bool Sprite::initVertexShader()
{
    Microsoft::WRL::ComPtr<ID3DBlob> vsBlob;

    HRESULT hr = D3DReadFileToBlob(L"scripts/shader/texture_pos_color_uv_vs.cso", &vsBlob);
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

bool Sprite::initInputLayout(ID3DBlob* vsBlob)
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

    D3D11_INPUT_ELEMENT_DESC uvDesc = {
        "TEXCOORD",
        1,
        DXGI_FORMAT_R32G32_FLOAT,
        0,
        offsetof(Vertex, uv),
        D3D11_INPUT_PER_VERTEX_DATA,
        0
    };

    D3D11_INPUT_ELEMENT_DESC layout[] = {
        positionDesc,
        colorDesc,
        uvDesc
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

bool Sprite::initPixelShader()
{
    Microsoft::WRL::ComPtr<ID3DBlob> psBlob;

    HRESULT hr = D3DReadFileToBlob(L"scripts/shader/texture_pos_color_uv_ps.cso", &psBlob);
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

bool Sprite::initTexture() 
{
    HRESULT hr = DirectX::CreateWICTextureFromFile(
        m_pContext->GetDevice(),
        L"images/SlimeBall.png",
        nullptr,
        &m_pTexture
    );
    if (FAILED(hr))
    {
        return false;
    }

    return true;
}

bool Sprite::initSampler()
{
    D3D11_SAMPLER_DESC desc = {};
    desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT; // ドット絵はPoint
    desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP; // 端が引き伸ばされる
    desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

    HRESULT hr = m_pContext->GetDevice()->CreateSamplerState(&desc, &m_pSamplerState);
    if (FAILED(hr))
    {
        return false;
    }

    return true;
}

bool Sprite::initDepthStencilNoMask()
{
    D3D11_DEPTH_STENCIL_DESC desc = {};
    desc.DepthEnable = TRUE;
    desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO; // マスク無し
    desc.DepthFunc = D3D11_COMPARISON_EQUAL; // 最初に書いたZのみのパスと一致したときだけ描画
    desc.StencilEnable = FALSE;
    desc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
    desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

    HRESULT hr = m_pContext->GetDevice()->CreateDepthStencilState(&desc, &m_pDepthStencilNoMaskState);
    if (FAILED(hr))
    {
        return false;
    }

    return true;
}

bool Sprite::initBlendNoColor()
{
    D3D11_BLEND_DESC desc = {};
    desc.RenderTarget[0].BlendEnable = FALSE;
    desc.RenderTarget[0].RenderTargetWriteMask = 0; // カラーを書かない

    HRESULT hr = m_pContext->GetDevice()->CreateBlendState(&desc, &m_pBlendNoColorState);
    if (FAILED(hr))
    {
        return false;
    }

    return true;
}

bool Sprite::initPixelShaderZOnly()
{
    Microsoft::WRL::ComPtr<ID3DBlob> psBlob;

    HRESULT hr = D3DReadFileToBlob(L"scripts/shader/z_only_pos_color_uv_ps.cso", &psBlob);
    if (FAILED(hr))
    {
        return false;
    }

    hr = m_pContext->GetDevice()->CreatePixelShader(
        psBlob->GetBufferPointer(),
        psBlob->GetBufferSize(),
        nullptr,
        &m_pPixelShaderZOnly
    );
    if (FAILED(hr))
    {
        return false;
    }

    psBlob = nullptr;

    return true;
}
