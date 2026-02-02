#pragma once
#include "renderable.h"

class RenderContext;
class Camera;

class Sphere : public Renderable
{
public:
    Sphere(RenderContext* pContext, Camera* pCamera);
    Sphere(RenderContext* pContext, Camera* pCamera, D3D11_PRIMITIVE_TOPOLOGY topology);
    ~Sphere() override;

    bool Initialize() override;
    void Start() override;
    void Update() override;
    void Draw() override;
    void Terminate() override;
    void Finalize() override;

protected:
    bool initVertexBuffer() override;
    bool initIndexBuffer() override;

private:
    struct Vertex
    {
        Vec3 pos;
        Vec3 normal;
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

    // 定数
    static const int LAT_DIV = 20;
    static const int LON_DIV = 20;
    static const Vec4 BLEND_FACTOR;
};
