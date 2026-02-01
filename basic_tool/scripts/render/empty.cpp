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
}

void Empty::Terminate()
{
}

void Empty::Finalize()
{
}