#pragma once
#include "renderer.h"
#include <wincodec.h>
#include <WICTextureLoader.h>

class RenderContext;
class Camera;

class Sprite : public Renderer
{
public:
    Sprite(RenderContext* pContext, Camera* pCamera, Transform* pTransform, const Vec4& color);
    ~Sprite() override;
    void Initialize() override;
    void Start() override;
    void Update() override;
    void Terminate() override;
    void Finalize() override;

protected:
    bool initRasterizer() override;
    bool initVertexBuffer() override;
    bool initIndexBuffer() override;
    bool initVertexShader() override;
    bool initInputLayout(ID3DBlob* vsBlob) override;
    bool initPixelShader() override;

private:
    struct Vertex // オブジェクトによって差異がなければ、共通で定義するかも
    {
        Vec3 pos;
        Vec4 color;
        Vec2 uv;
    };
    static const int VERTEX_COUNT = 4;

    // シェーダーに必要な分だけ増やす
    bool initConstantBufferA();
    void updateConstantBufferA();
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_pConstantBufferA = nullptr;
    struct ConstantBufferA
    {
        float world[4][4];
        float view[4][4];
        float proj[4][4];
    };

    Vec4 m_color;

    bool initSampler();
    bool initTexture();
    Microsoft::WRL::ComPtr<ID3D11SamplerState> m_pSamplerState = nullptr;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pTexture = nullptr;

    bool initDepthStencilNoMask();
    bool initBlendNoColor();
    bool initPixelShaderZOnly();
    Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_pDepthStencilNoMaskState = nullptr;
    Microsoft::WRL::ComPtr<ID3D11BlendState> m_pBlendNoColorState = nullptr;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShaderZOnly = nullptr;

    // 定数
    static const Vec4 BLEND_FACTOR;
};