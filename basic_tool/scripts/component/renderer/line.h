#pragma once
#include "renderer.h"

class RenderContext;
class Camera;

class Line : public Renderer
{
public:
    Line(const RenderContext* const pContext, const Camera* const pCamera, const Vec4& color);
    ~Line() override;

    void Initialize() override;
    void Start() override;
    void Update() override;
    void Terminate() override;
    void Finalize() override;
    void SetLine(const Vec3& start, const Vec3& end);
    void Show() override;

protected:
    bool initVertexBuffer() override;
    bool initIndexBuffer() override;
    bool initVertexShader() override;
    bool initInputLayout(ID3DBlob* vsBlob) override;
    bool initPixelShader() override;
    bool initDepthStencil();

private:
    struct Vertex
    {
        Vec3 pos;
        Vec4 color;
    };
    static const int VERTEX_COUNT = 2;

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

    // 定数
    static const Vec4 BLEND_FACTOR;
};