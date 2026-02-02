#pragma once
#include "renderable.h"
#include <memory>

class RenderContext;

class Empty : public Renderable
{
public:
    Empty(std::shared_ptr<RenderContext> pContext);
    Empty(std::shared_ptr<RenderContext> pContext, D3D11_PRIMITIVE_TOPOLOGY topology);
    ~Empty() override;
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
    Vertex m_vertices[VERTEX_COUNT];
    
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
};