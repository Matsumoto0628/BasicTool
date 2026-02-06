#pragma once
#include "renderer.h"

class RenderContext;
class Camera;
class Transform;

class TriangleTest : public Renderer
{
public:
    TriangleTest(RenderContext* pContext, Camera* pCamera, Transform* pTransform);
    TriangleTest(RenderContext* pContext, Camera* pCamera, Transform* pTransform, D3D11_PRIMITIVE_TOPOLOGY topology);
    ~TriangleTest() override;
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
    struct Vertex // オブジェクトによって差異がなければ、共通で定義するかも
    {
        Vec3 pos;
    };
    static const int VERTEX_COUNT = 3;
    
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

    // 定数
    static const Vec4 BLEND_FACTOR;
};