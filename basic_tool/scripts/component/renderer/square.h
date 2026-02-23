#pragma once
#include "renderer.h"

class RenderContext;
class Camera;

class Square : public Renderer
{
public:
    Square(const RenderContext* const pContext, const Camera* const pCamera, const Transform* const pTransform, const Vec4& color);
    ~Square() override;
    void Initialize() override;
    void Update() override;
    void Finalize() override;
    void Show() override;

protected:
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
    };
    struct ConstantBufferA
    {
        float world[4][4];
        float view[4][4];
        float proj[4][4];
    };
    bool initConstantBufferA();
    void updateConstantBufferA();
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_pConstantBufferA = nullptr;
    Vec4 m_color;

    // 定数
    static const int VERTEX_COUNT = 4;
    static const UINT STRIDE = sizeof(Vertex);
    static const UINT OFFSET = 0;
    static const Vec4 BLEND_FACTOR;
};