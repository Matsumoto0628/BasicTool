/**
 * @file line.h
 * @brief ライン（線分）レンダラーコンポーネント
 * @author Matsumoto0628
 * @date 2026-05-03
 */
#pragma once
#include "renderer.h"

class RenderContext;
class Camera;

/**
 * @brief 2 点間を結ぶ線分を描画するレンダラーコンポーネント
 */
class Line : public Renderer
{
public:
    /**
     * @brief デシリアライズ用コンストラクタ
     * @param id       コンポーネント ID
     * @param pContext レンダリングコンテキスト
     * @param pCamera  使用するカメラ
     * @param color    ライン色（RGBA）
     */
    Line(uint64_t id, const RenderContext* const pContext, const Camera* const pCamera, const Vec4& color);
    /**
     * @brief 通常生成用コンストラクタ
     * @param pContext レンダリングコンテキスト
     * @param pCamera  使用するカメラ
     * @param color    ライン色（RGBA）
     */
    Line(const RenderContext* const pContext, const Camera* const pCamera, const Vec4& color);
    ~Line() override;

    void Initialize() override;
    void Start() override;
    void Update() override;
    void Draw() override;
    void Finalize() override;
    /**
     * @brief ラインの始点・終点を設定する
     * @param start 始点座標（ワールド空間）
     * @param end   終点座標（ワールド空間）
     */
    void SetLine(const Vec3& start, const Vec3& end);
    void Show() override;
    /** @brief JSON にシリアライズして返す */
    Json Serialize() const override;
    /**
     * @brief JSON から Line を生成して返す
     * @param j        JSON オブジェクト
     * @param pContext レンダリングコンテキスト
     */
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
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_pConstantBufferA = nullptr; ///< 定数バッファ（変換行列）
    Vec4 m_color;                                                       ///< ライン色

    static const int  VERTEX_COUNT = 2;          ///< 頂点数（始点・終点）
    static const UINT STRIDE       = sizeof(Vertex);
    static const UINT OFFSET       = 0;
    static const Vec4 BLEND_FACTOR;
};
