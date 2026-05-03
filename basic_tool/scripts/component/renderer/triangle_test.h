/**
 * @file triangle_test.h
 * @brief 三角形テスト用レンダラーコンポーネント
 * @author Matsumoto0628
 * @date 2026-05-03
 */
#pragma once
#include "renderer.h"
#include <memory>

class RenderContext;
class Camera;
class Transform;

/**
 * @brief 動作確認用の単純な三角形を描画するレンダラーコンポーネント
 */
class TriangleTest : public Renderer
{
public:
    /**
     * @brief デシリアライズ用コンストラクタ
     * @param id         コンポーネント ID
     * @param pContext   レンダリングコンテキスト
     * @param pCamera    使用するカメラ
     * @param pTransform トランスフォーム
     */
    TriangleTest(uint64_t id, const RenderContext* const pContext, const Camera* const pCamera, const Transform* const pTransform);
    /**
     * @brief 通常生成用コンストラクタ
     * @param pContext   レンダリングコンテキスト
     * @param pCamera    使用するカメラ
     * @param pTransform トランスフォーム
     */
    TriangleTest(const RenderContext* const pContext, const Camera* const pCamera, const Transform* const pTransform);
    ~TriangleTest() override;
    void Initialize() override;
    void Start() override;
    void Update() override;
    void Draw() override;
    void Finalize() override;
    void Show() override;
    /** @brief JSON にシリアライズして返す */
    Json Serialize() const override;
    /**
     * @brief JSON から TriangleTest を生成して返す
     * @param j          JSON オブジェクト
     * @param pContext   レンダリングコンテキスト
     * @param pTransform トランスフォーム
     */
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
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_pConstantBufferA = nullptr; ///< 定数バッファ（変換行列）

    static const int  VERTEX_COUNT = 3;          ///< 頂点数
    static const UINT STRIDE       = sizeof(Vertex);
    static const UINT OFFSET       = 0;
    static const Vec4 BLEND_FACTOR;
};
