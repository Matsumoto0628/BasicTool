/**
 * @file sprite.h
 * @brief スプライト（テクスチャ付き四角形）レンダラーコンポーネント
 * @author Matsumoto0628
 * @date 2026-05-03
 */
#pragma once
#include "renderer.h"
#include <wincodec.h>
#include <WICTextureLoader.h>
#include <string>

class RenderContext;
class Camera;

/**
 * @brief テクスチャを貼ったビルボードスプライトを描画するレンダラーコンポーネント
 */
class Sprite : public Renderer
{
public:
    /**
     * @brief デシリアライズ用コンストラクタ
     * @param id         コンポーネント ID
     * @param pContext   レンダリングコンテキスト
     * @param pCamera    使用するカメラ
     * @param pTransform トランスフォーム
     * @param color      乗算カラー（RGBA）
     */
    Sprite(uint64_t id, const RenderContext* const pContext, const Camera* const pCamera, const Transform* const pTransform, const Vec4& color);
    /**
     * @brief 通常生成用コンストラクタ
     * @param pContext   レンダリングコンテキスト
     * @param pCamera    使用するカメラ
     * @param pTransform トランスフォーム
     * @param color      乗算カラー（RGBA）
     */
    Sprite(const RenderContext* const pContext, const Camera* const pCamera, const Transform* const pTransform, const Vec4& color);
    ~Sprite() override;
    void Initialize() override;
    void Start() override;
    void Update() override;
    void Draw() override;
    void Finalize() override;
    void Show() override;
    /** @brief JSON にシリアライズして返す */
    Json Serialize() const override;
    /**
     * @brief JSON から Sprite を生成して返す
     * @param j          JSON オブジェクト
     * @param pContext   レンダリングコンテキスト
     * @param pTransform トランスフォーム
     */
    static std::unique_ptr<Sprite> Deserialize(const Json& j, const RenderContext* const pContext, const Transform* const pTransform);
    /**
     * @brief テクスチャを WIC でロードして設定する
     * @param path テクスチャファイルパス
     * @return 成功したら true
     */
    bool SetTexture(const std::wstring& path);
    /**
     * @brief 乗算カラーを設定する
     * @param color RGBA カラー
     */
    void SetColor(const Vec4& color) { m_color = color; }
    /** @brief 現在の乗算カラーを返す */
    const Vec4& GetColor() const { return m_color; }

protected:
    bool initRasterizer() override;
    bool initVertexBuffer() override;
    bool initIndexBuffer() override;
    bool initVertexShader() override;
    bool initInputLayout(ID3DBlob* vsBlob) override;
    bool initPixelShader() override;

private:
    struct Vertex
    {
        Vec3 pos;
        Vec2 uv;
    };
    struct ConstantBufferA
    {
        float world[4][4];
        float view[4][4];
        float proj[4][4];
    };
    struct ConstantBufferB
    {
        float color[4];
    };
    bool initSampler();
    bool initTexture();
    bool initDepthStencilNoMask();
    bool initBlendNoColor();
    bool initPixelShaderZOnly();
    bool initConstantBufferA();
    bool initConstantBufferB();
    void updateConstantBufferA();
    void updateConstantBufferB();

    Microsoft::WRL::ComPtr<ID3D11SamplerState>      m_pSamplerState          = nullptr; ///< サンプラーステート
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pTexture              = nullptr; ///< テクスチャ SRV
    Microsoft::WRL::ComPtr<ID3D11DepthStencilState>  m_pDepthStencilNoMaskState = nullptr; ///< マスクなし深度ステンシル
    Microsoft::WRL::ComPtr<ID3D11BlendState>         m_pBlendNoColorState    = nullptr; ///< カラー書き込みなしブレンド
    Microsoft::WRL::ComPtr<ID3D11PixelShader>        m_pPixelShaderZOnly     = nullptr; ///< Z 書き込み専用ピクセルシェーダー
    Microsoft::WRL::ComPtr<ID3D11Buffer>             m_pConstantBufferA      = nullptr; ///< 定数バッファ（変換行列）
    Microsoft::WRL::ComPtr<ID3D11Buffer>             m_pConstantBufferB      = nullptr; ///< 定数バッファ（カラー）
    Vec4 m_color;                                                                        ///< 乗算カラー

    static const int  VERTEX_COUNT = 4;          ///< 頂点数
    static const UINT STRIDE       = sizeof(Vertex);
    static const UINT OFFSET       = 0;
    static const Vec4 BLEND_FACTOR;
};
