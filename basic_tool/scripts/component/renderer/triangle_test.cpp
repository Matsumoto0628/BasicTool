#include "triangle_test.h"
#include "render_context.h"
#include "camera.h"
#include "transform.h"
#include "game_random.h"
#include "scene_manager.h"

// 定数
const Vec4 TriangleTest::BLEND_FACTOR = { 0, 0, 0, 0 };

TriangleTest::TriangleTest(uint64_t id, const RenderContext* const pContext, const Camera* const pCamera, const Transform* const pTransform)
    : Renderer{ id, Type::Triangle, pContext, pCamera, pTransform, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST }
{
}

TriangleTest::TriangleTest(const RenderContext* const pContext, const Camera* const pCamera, const Transform* const pTransform)
	: TriangleTest{ GameRandom::GetUUID(), pContext, pCamera, pTransform }
{
}

TriangleTest::~TriangleTest()
{
}

void TriangleTest::Initialize() 
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

void TriangleTest::Start()
{
}

void TriangleTest::Update()
{
}

void TriangleTest::Draw()
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

        getContext()->GetDeviceContext()->Draw(VERTEX_COUNT, 0);
    }
}

void TriangleTest::Finalize()
{
}

void TriangleTest::Show()
{
}

bool TriangleTest::initVertexBuffer() 
{
    Vertex vertices[VERTEX_COUNT] = {
        { Vec3(0, 1, 0) },
        { Vec3(1,-1,0) },
        { Vec3(-1,-1,0) }
    };

    D3D11_BUFFER_DESC desc = {};
    desc.ByteWidth = sizeof(vertices);
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    desc.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = vertices;

    HRESULT hr = getContext()->GetDevice()->CreateBuffer(
        &desc,
        &initData,
        &m_pVertexBuffer
    );
    if (FAILED(hr)) 
    {
        return false;
    }

    return true;
}

bool TriangleTest::initIndexBuffer()
{
    unsigned int indices[] = { 0,1,2 };

    D3D11_BUFFER_DESC desc = {};
    desc.ByteWidth = sizeof(indices);
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = indices;

    HRESULT hr = getContext()->GetDevice()->CreateBuffer(
        &desc,
        &initData,
        &m_pIndexBuffer
    );
    if (FAILED(hr))
    {
        return false;
    }

    return true;
}

bool TriangleTest::initConstantBufferA() 
{
    D3D11_BUFFER_DESC desc = {};
    desc.ByteWidth = sizeof(ConstantBufferA);
    desc.Usage = D3D11_USAGE_DYNAMIC; // Map/Unmapで書き換える
    desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    HRESULT hr = getContext()->GetDevice()->CreateBuffer(
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

void TriangleTest::updateConstantBufferA() 
{
    // 渡すもの
    ConstantBufferA cb;
    getTransform()->GetMatrix().Transpose().ToFloat4x4(cb.world);
    getCamera()->GetView().Transpose().ToFloat4x4(cb.view);
    getCamera()->GetProj().Transpose().ToFloat4x4(cb.proj);

    D3D11_MAPPED_SUBRESOURCE mapped = {};

    HRESULT hr = getContext()->GetDeviceContext()->Map(
        m_pConstantBufferA.Get(),
        0,
        D3D11_MAP_WRITE_DISCARD,
        0,
        &mapped
    );
    if (SUCCEEDED(hr))
    {
        memcpy(mapped.pData, &cb, sizeof(ConstantBufferA));
        getContext()->GetDeviceContext()->Unmap(m_pConstantBufferA.Get(), 0);
    }
}

Json TriangleTest::Serialize() const
{
    return {
        { "id", GetID()},
        { "type", GetType()},
        { "camera", getCamera()->GetID() }
    };
}

std::unique_ptr<TriangleTest> TriangleTest::Deserialize(const Json& j, const RenderContext* const pContext, const Transform* const pTransform)
{
    auto pComponent = std::make_unique<TriangleTest>(
        j.at("id").get<uint64_t>(),
        pContext,
        SceneManager::GetCurrentScene()->FindComponent<Camera>(j.at("camera").get<uint64_t>()),
        pTransform
    );
    return pComponent;
}
