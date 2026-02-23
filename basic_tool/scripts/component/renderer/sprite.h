#pragma once
#include "renderer.h"
#include <wincodec.h>
#include <WICTextureLoader.h>

class RenderContext;
class Camera;

class Sprite : public Renderer
{
public:
    Sprite(const RenderContext* const pContext, const Camera* const pCamera, const Transform* const pTransform, const Vec4& color);
    ~Sprite() override;
    void Initialize() override;
    void Update() override;
    void Finalize() override;
    void Show() override;

protected:
    bool initRasterizer() override;
    bool initVertexBuffer() override;
    bool initIndexBuffer() override;
    bool initVertexShader() override;
    bool initInputLayout(ID3DBlob* vsBlob) override;
    bool initPixelShader() override;

private:
    struct Vertex
    {
        Vec3 pos;
        Vec4 color;
        Vec2 uv;
    };
    struct ConstantBufferA
    {
        float world[4][4];
        float view[4][4];
        float proj[4][4];
    };
    bool initSampler();
    bool initTexture();
    bool initDepthStencilNoMask();
    bool initBlendNoColor();
    bool initPixelShaderZOnly();
    bool initConstantBufferA();
    void updateConstantBufferA();
    Microsoft::WRL::ComPtr<ID3D11SamplerState> m_pSamplerState = nullptr;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pTexture = nullptr;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_pDepthStencilNoMaskState = nullptr;
    Microsoft::WRL::ComPtr<ID3D11BlendState> m_pBlendNoColorState = nullptr;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShaderZOnly = nullptr;
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_pConstantBufferA = nullptr;
    Vec4 m_color;

    // 定数
    static const int VERTEX_COUNT = 4;
    static const UINT STRIDE = sizeof(Vertex);
    static const UINT OFFSET = 0;
    static const Vec4 BLEND_FACTOR;
};