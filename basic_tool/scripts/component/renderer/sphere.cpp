#include "sphere.h"
#include "render_context.h"
#include <vector>
#include "camera.h"
#include "transform.h"
#include "game_random.h"
#include "scene_manager.h"

const Vec4 Sphere::BLEND_FACTOR = { 0,0,0,0 };

Sphere::Sphere(uint64_t id, const RenderContext* const pContext, const Camera* const pCamera, const Transform* const pTransform, const Vec4& color)
    : Renderer{ id, Type::Sphere, pContext, pCamera, pTransform, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST }, m_color{ color }
{
}

Sphere::Sphere(const RenderContext* const pContext, const Camera* const pCamera, const Transform* const pTransform, const Vec4& color)
    : Sphere{GameRandom::GetUUID(), pContext, pCamera, pTransform, color}
{
}

Sphere::~Sphere()
{
}

void Sphere::Initialize()
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

void Sphere::Start()
{
}

void Sphere::Update() 
{ 
}

void Sphere::Draw()
{
    // 更新
    {
        updateConstantBufferA();
    }

    // 描画
    {
        float blendFactor[4];
        BLEND_FACTOR.ToFloat4(blendFactor);

        getContext()->GetDeviceContext()->VSSetShader(m_pVertexShader.Get(), nullptr, 0);
        getContext()->GetDeviceContext()->IASetInputLayout(m_pInputLayout.Get());
        getContext()->GetDeviceContext()->PSSetShader(m_pPixelShader.Get(), nullptr, 0);
        getContext()->GetDeviceContext()->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &STRIDE, &OFFSET);
        getContext()->GetDeviceContext()->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
        getContext()->GetDeviceContext()->VSSetConstantBuffers(0, 1, m_pConstantBufferA.GetAddressOf());
        getContext()->GetDeviceContext()->OMSetBlendState(m_pBlendState.Get(), blendFactor, 0xffffffff);
        getContext()->GetDeviceContext()->OMSetDepthStencilState(m_pDepthStencilState.Get(), 0);
        getContext()->GetDeviceContext()->RSSetState(m_pRasterizerState.Get());
        getContext()->GetDeviceContext()->IASetPrimitiveTopology(getTopology());

        getContext()->GetDeviceContext()->DrawIndexed(LAT_DIV * LON_DIV * 6, 0, 0);
    }
}

void Sphere::Finalize() 
{
}

void Sphere::Show()
{
}

bool Sphere::initVertexBuffer()
{
    std::vector<Vertex> vertices((LAT_DIV + 1) * (LON_DIV + 1));

    for (int i = 0; i <= LAT_DIV; i++)
    {
        float theta = i * PI / LAT_DIV;
        float sinTheta = sinf(theta);
        float cosTheta = cosf(theta);

        for (int j = 0; j <= LON_DIV; j++)
        {
            float phi = j * 2.0f * PI / LON_DIV;
            float sinPhi = sinf(phi);
            float cosPhi = cosf(phi);

            int index = i * (LON_DIV + 1) + j;
            vertices[index].pos = Vec3(sinTheta * cosPhi, cosTheta, sinTheta * sinPhi);
            vertices[index].color = m_color;
        }
    }

    D3D11_BUFFER_DESC desc = {};
    desc.ByteWidth = sizeof(Vertex) * vertices.size();
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = vertices.data();

    return SUCCEEDED(getContext()->GetDevice()->CreateBuffer(&desc, &initData, &m_pVertexBuffer));
}

bool Sphere::initIndexBuffer()
{
    std::vector<unsigned int> indices;

    for (int i = 0; i < LAT_DIV; i++)
    {
        for (int j = 0; j < LON_DIV; j++)
        {
            int first = i * (LON_DIV + 1) + j;
            int second = first + LON_DIV + 1;

            indices.push_back(first);
            indices.push_back(second);
            indices.push_back(first + 1);

            indices.push_back(second);
            indices.push_back(second + 1);
            indices.push_back(first + 1);
        }
    }

    D3D11_BUFFER_DESC desc = {};
    desc.ByteWidth = sizeof(unsigned int) * indices.size();
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = indices.data();

    return SUCCEEDED(getContext()->GetDevice()->CreateBuffer(&desc, &initData, &m_pIndexBuffer));
}

bool Sphere::initConstantBufferA()
{
    D3D11_BUFFER_DESC desc = {};
    desc.ByteWidth = sizeof(ConstantBufferA);
    desc.Usage = D3D11_USAGE_DYNAMIC;
    desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    return SUCCEEDED(getContext()->GetDevice()->CreateBuffer(&desc, nullptr, &m_pConstantBufferA));
}

void Sphere::updateConstantBufferA()
{
    ConstantBufferA cb;
    getTransform()->GetMatrix().Transpose().ToFloat4x4(cb.world);
    getCamera()->GetView().Transpose().ToFloat4x4(cb.view);
    getCamera()->GetProj().Transpose().ToFloat4x4(cb.proj);

    D3D11_MAPPED_SUBRESOURCE mapped = {};
    if (SUCCEEDED(getContext()->GetDeviceContext()->Map(m_pConstantBufferA.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped)))
    {
        memcpy(mapped.pData, &cb, sizeof(ConstantBufferA));
        getContext()->GetDeviceContext()->Unmap(m_pConstantBufferA.Get(), 0);
    }
}

Json Sphere::Serialize() const
{
    return {
        {"id", GetID()},
        {"type", GetType()},
        { "color", m_color },
        {"camera", getCamera()->GetID() }
    };
}

std::unique_ptr<Sphere> Sphere::Deserialize(const Json& j, const RenderContext* const pContext, const Transform* const pTransform)
{
    auto pComponent = std::make_unique<Sphere>(
        j.at("id").get<uint64_t>(),
        pContext,
        SceneManager::GetCurrentScene()->FindComponent<Camera>(j.at("camera").get<uint64_t>()),
		pTransform,
        j.at("color").get<Vec4>()
    );
    return pComponent;
}

bool Sphere::initVertexShader()
{
    Microsoft::WRL::ComPtr<ID3DBlob> vsBlob;

    HRESULT hr = D3DReadFileToBlob(L"C:/Users/kanik/Desktop/DirectXProject/basic_tool/basic_tool/scripts/shader/test_pos_color_vs.cso", &vsBlob);
    if (FAILED(hr))
    {
        return false;
    }

    hr = getContext()->GetDevice()->CreateVertexShader(
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

bool Sphere::initInputLayout(ID3DBlob* vsBlob)
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

    HRESULT hr = getContext()->GetDevice()->CreateInputLayout(
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

bool Sphere::initPixelShader()
{
    Microsoft::WRL::ComPtr<ID3DBlob> psBlob;

    HRESULT hr = D3DReadFileToBlob(L"C:/Users/kanik/Desktop/DirectXProject/basic_tool/basic_tool/scripts/shader/test_pos_color_ps.cso", &psBlob);
    if (FAILED(hr))
    {
        return false;
    }

    hr = getContext()->GetDevice()->CreatePixelShader(
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