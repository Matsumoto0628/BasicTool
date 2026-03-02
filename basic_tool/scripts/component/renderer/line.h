#pragma once
#include "renderer.h"

class RenderContext;
class Camera;

class Line : public Renderer
{
public:
    Line(uint64_t id, const RenderContext* const pContext, const Camera* const pCamera, const Vec4& color);
    Line(const RenderContext* const pContext, const Camera* const pCamera, const Vec4& color);
    ~Line() override;

    void Initialize() override;
    void Start() override;
    void Update() override;
    void Draw() override;
    void Finalize() override;
    void SetLine(const Vec3& start, const Vec3& end);
    void Show() override;
    Json Serialize() const override;
	static std::unique_ptr<Line> Deserialize(const Json& j, const RenderContext* const pContext);

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
    static const int VERTEX_COUNT = 2;
    static const UINT STRIDE = sizeof(Vertex);
    static const UINT OFFSET = 0;
    static const Vec4 BLEND_FACTOR;
};