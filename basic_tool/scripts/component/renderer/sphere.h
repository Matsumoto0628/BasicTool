#pragma once
#include "renderer.h"

class RenderContext;
class Camera;
class Transform;

class Sphere : public Renderer
{
public:
    Sphere(RenderContext* pContext, Camera* pCamera, Transform* pTransform);
    Sphere(RenderContext* pContext, Camera* pCamera, Transform* pTransform, D3D11_PRIMITIVE_TOPOLOGY topology);
    Sphere(RenderContext* pContext, Camera* pCamera, Transform* pTransform, Vec4 color);
    ~Sphere() override;

    bool Initialize() override;
    void Start() override;
    void Update() override;
    void Terminate() override;
    void Finalize() override;

protected:
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
    };

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
    static const int LAT_DIV = 20;
    static const int LON_DIV = 20;
    static const Vec4 BLEND_FACTOR;
};
