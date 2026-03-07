#pragma once
#include "renderer.h"
#include <memory>

class RenderContext;
class Camera;
class Transform;

class TriangleTest : public Renderer
{
public:
    TriangleTest(uint64_t id, const RenderContext* const pContext, const Camera* const pCamera, const Transform* const pTransform);
    TriangleTest(const RenderContext* const pContext, const Camera* const pCamera, const Transform* const pTransform);
    ~TriangleTest() override;
    void Initialize() override;
    void Start() override;
    void Update() override;
    void Draw() override;
    void Finalize() override;
    void Show() override;
    Json Serialize() const override;
    static std::unique_ptr<TriangleTest> Deserialize(const Json& j, const RenderContext* const pContext, const Transform* const pTransform);

protected:
    bool initVertexBuffer() override;
    bool initIndexBuffer() override;

private:
    struct Vertex
    {
        Vec3 pos;
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

    // 定数
    static const int VERTEX_COUNT = 3;
    static const UINT STRIDE = sizeof(Vertex);
    static const UINT OFFSET = 0;
    static const Vec4 BLEND_FACTOR;
};