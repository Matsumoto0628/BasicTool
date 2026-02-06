#pragma once
#include "renderer.h"
#include <wincodec.h>
#include <WICTextureLoader.h>

class RenderContext;
class Camera;

class Square : public Renderer
{
public:
    Square(RenderContext* pContext, Camera* pCamera, Transform* pTransform);
    Square(RenderContext* pContext, Camera* pCamera, Transform* pTransform, D3D11_PRIMITIVE_TOPOLOGY topology);
    ~Square() override;
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

    bool initSampler();
    bool initTexture();
    Microsoft::WRL::ComPtr<ID3D11SamplerState> m_pSamplerState;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pTexture;

    // 定数
    static const Vec4 BLEND_FACTOR;
};