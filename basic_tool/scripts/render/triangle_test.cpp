#include "triangle_test.h"
#include "render_context.h"
#include "camera.h"

// 定数
const Vec4 TriangleTest::BLEND_FACTOR = { 0, 0, 0, 0 };

TriangleTest::TriangleTest(RenderContext* pContext, Camera* pCamera)
    : Renderable(pContext, pCamera) // protectedのメンバ変数は基底クラスで初期化
{
}

TriangleTest::TriangleTest(RenderContext* pContext, Camera* pCamera, D3D11_PRIMITIVE_TOPOLOGY topology)
    : Renderable(pContext, pCamera, topology)
{
}

TriangleTest::~TriangleTest()
{
    m_pContext = nullptr;
    m_pCamera = nullptr;
}

bool TriangleTest::Initialize() 
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

    // ここでInputLayoutも初期化
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
        bool result = initVertexBuffer();
        if (!result)
        {
            return false;
        }
    }

    {
        bool result = initIndexBuffer();
        if (!result)
        {
            return false;
        }
    }

    {
        bool result = initConstantBufferA();
        if (!result)
        {
            return false;
        }
    }

    return true;
}

void TriangleTest::Start()
{
}

void TriangleTest::Update()
{
    updateConstantBufferA();
}

void TriangleTest::Draw()
{
    UINT stride = sizeof(Vertex);
    UINT offset = 0;

    m_pContext->GetDeviceContext()->VSSetShader(m_pVertexShader.Get(), nullptr, 0);
    m_pContext->GetDeviceContext()->IASetInputLayout(m_pInputLayout.Get());
    m_pContext->GetDeviceContext()->PSSetShader(m_pPixelShader.Get(), nullptr, 0);
    m_pContext->GetDeviceContext()->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &stride, &offset);
    m_pContext->GetDeviceContext()->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
    m_pContext->GetDeviceContext()->VSSetConstantBuffers(0, 1, m_pConstantBufferA.GetAddressOf());
    m_pContext->GetDeviceContext()->OMSetBlendState(m_pBlendState.Get(), blendFactor, 0xffffffff);
    m_pContext->GetDeviceContext()->OMSetDepthStencilState(m_pDepthStencilState.Get(), 0);
    m_pContext->GetDeviceContext()->IASetPrimitiveTopology(m_topology);

    m_pContext->GetDeviceContext()->Draw(VERTEX_COUNT, 0);
}

void TriangleTest::Terminate()
{
}

void TriangleTest::Finalize()
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

    HRESULT hr = m_pContext->GetDevice()->CreateBuffer(
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

    HRESULT hr = m_pContext->GetDevice()->CreateBuffer(
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

void TriangleTest::updateConstantBufferA() 
{
    // 渡すもの
    ConstantBufferA cb;
    m_transform.Matrix().Transpose().ToFloat4x4(cb.world);
    m_pCamera->GetView().Transpose().ToFloat4x4(cb.view);
    m_pCamera->GetProj().Transpose().ToFloat4x4(cb.proj);

    D3D11_MAPPED_SUBRESOURCE mapped = {};

    HRESULT hr = m_pContext->GetDeviceContext()->Map(
        m_pConstantBufferA.Get(),
        0,
        D3D11_MAP_WRITE_DISCARD,
        0,
        &mapped
    );
    if (SUCCEEDED(hr))
    {
        memcpy(mapped.pData, &cb, sizeof(ConstantBufferA));
        m_pContext->GetDeviceContext()->Unmap(m_pConstantBufferA.Get(), 0);
    }
}