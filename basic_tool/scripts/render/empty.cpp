#include "empty.h"
#include "render_context.h"

// 定数
const Vec4 Empty::BLEND_FACTOR = { 0, 0, 0, 0 };

Empty::Empty(std::shared_ptr<RenderContext> pContext)
    : Renderable(std::move(pContext)) // protectedのメンバ変数は基底クラスで初期化
{
}

Empty::Empty(std::shared_ptr<RenderContext> pContext, D3D11_PRIMITIVE_TOPOLOGY topology)
    : Renderable(std::move(pContext), topology)
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

    m_pContext->GetDeviceContext()->VSSetShader(m_pVertexShader.Get(), nullptr, 0);
    m_pContext->GetDeviceContext()->IASetInputLayout(m_pInputLayout.Get());
    m_pContext->GetDeviceContext()->PSSetShader(m_pPixelShader.Get(), nullptr, 0);
    m_pContext->GetDeviceContext()->IASetPrimitiveTopology(m_topology);

    return true;
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

    m_pContext->GetDeviceContext()->OMSetBlendState(m_pBlendState.Get(), blendFactor, 0xffffffff);
    m_pContext->GetDeviceContext()->OMSetDepthStencilState(m_pDepthStencilState.Get(), 0);
    //m_pContext->GetDeviceContext()->Draw();
}

void Empty::Terminate()
{
}

void Empty::Finalize()
{
}

bool Empty::initVertexBuffer() 
{
    return true;
}

bool Empty::initIndexBuffer()
{
    return true;
}

void Empty::updateConstantBufferA() 
{
}