#pragma once
#include "renderer.h"

class RenderContext;
class Camera;
class Transform;

class Sphere : public Renderer
{
public:
    Sphere(uint64_t id, const RenderContext* const pContext, const Camera* const pCamera, const Transform* const pTransform, const Vec4& color);
    Sphere(const RenderContext* const pContext, const Camera* const pCamera, const Transform* const pTransform, const Vec4& color);
    ~Sphere() override;

    void Initialize() override;
    void Start() override;
    void Update() override;
    void Draw() override;
    void Finalize() override;
    void Show() override;
    Json Serialize() const override;
	static std::unique_ptr<Sphere> Deserialize(const Json& j, const RenderContext* const pContext, const Transform* const pTransform);

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
    static const int LAT_DIV = 20;
    static const int LON_DIV = 20;
    static const UINT STRIDE = sizeof(Vertex);
    static const UINT OFFSET = 0;
    static const Vec4 BLEND_FACTOR;
};
