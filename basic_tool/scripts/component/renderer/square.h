/**
 * @file square.h
 * @brief スクエア（四角形）レンダラーコンポーネント
 * @author Matsumoto0628
 * @date 2026-05-03
 */
#pragma once
#include "renderer.h"

class RenderContext;
class Camera;

/**
 * @brief 単色の四角形ポリゴンを描画するレンダラーコンポーネント
 */
class Square : public Renderer
{
public:
    /**
     * @brief デシリアライズ用コンストラクタ
     * @param id         コンポーネント ID
     * @param pContext   レンダリングコンテキスト
     * @param pCamera    使用するカメラ
     * @param pTransform トランスフォーム
     * @param color      四角形色（RGBA）
     */
    Square(uint64_t id, const RenderContext* const pContext, const Camera* const pCamera, const Transform* const pTransform, const Vec4& color);
    /**
     * @brief 通常生成用コンストラクタ
     * @param pContext   レンダリングコンテキスト
     * @param pCamera    使用するカメラ
     * @param pTransform トランスフォーム
     * @param color      四角形色（RGBA）
     */
    Square(const RenderContext* const pContext, const Camera* const pCamera, const Transform* const pTransform, const Vec4& color);
    ~Square() override;
    void Initialize() override;
    void Start() override;
    void Update() override;
    void Draw() override;
    void Finalize() override;
    void Show() override;
    /** @brief JSON にシリアライズして返す */
    Json Serialize() const override;
    /**
     * @brief JSON から Square を生成して返す
     * @param j          JSON オブジェクト
     * @param pContext   レンダリングコンテキスト
     * @param pTransform トランスフォーム
     */
    static std::unique_ptr<Square> Deserialize(const Json& j, const RenderContext* const pContext, const Transform* const pTransform);

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
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_pConstantBufferA = nullptr; ///< 定数バッファ（変換行列）
    Vec4 m_color;                                                       ///< 四角形色

    static const int  VERTEX_COUNT = 4;          ///< 頂点数
    static const UINT STRIDE       = sizeof(Vertex);
    static const UINT OFFSET       = 0;
    static const Vec4 BLEND_FACTOR;
};
