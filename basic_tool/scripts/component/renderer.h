/**
 * @file renderer.h
 * @brief レンダラーコンポーネント基底クラス
 * @author Matsumoto0628
 * @date 2026-05-03
 */
#pragma once
#include "component.h"

class RenderContext;
class Camera;
class Transform;

/**
 * @brief DirectX11 描画に共通する初期化処理を提供するレンダラー基底クラス
 *
 * Component の仮想関数は派生クラスで実装する。
 * 共通リソース（シェーダー等）は protected メンバーとして派生クラスから直接利用できる。
 */
class Renderer : public Component
{
public:
    virtual ~Renderer() = default;

protected:
    /**
     * @brief コンストラクタ
     * @param id         コンポーネント ID
     * @param type       コンポーネント種別
     * @param pContext   レンダリングコンテキスト
     * @param pCamera    使用するカメラ
     * @param pTransform 使用するトランスフォーム
     * @param topology   プリミティブトポロジー
     */
    Renderer(
        uint64_t id,
        const Type type,
        const RenderContext* const pContext,
        const Camera* const pCamera,
        const Transform* const pTransform,
        D3D11_PRIMITIVE_TOPOLOGY topology
    );
    /** @brief 深度ステンシルステートを初期化する */
    virtual bool initDepthStencil();
    /** @brief ブレンドステートを初期化する */
    virtual bool initBlend();
    /** @brief ラスタライザーステートを初期化する */
    virtual bool initRasterizer();
    /** @brief 頂点シェーダーをコンパイルして初期化する */
    virtual bool initVertexShader();
    /**
     * @brief 入力レイアウトを初期化する
     * @param vsBlob コンパイル済み頂点シェーダーの BLOB
     */
    virtual bool initInputLayout(ID3DBlob* vsBlob);
    /** @brief ピクセルシェーダーをコンパイルして初期化する */
    virtual bool initPixelShader();
    /** @brief 頂点バッファを初期化する（純粋仮想） */
    virtual bool initVertexBuffer() = 0;
    /** @brief インデックスバッファを初期化する（純粋仮想） */
    virtual bool initIndexBuffer() = 0;

    /** @brief プリミティブトポロジーを返す */
    D3D11_PRIMITIVE_TOPOLOGY getTopology() const { return m_topology; }
    /** @brief レンダリングコンテキストを返す */
    const RenderContext* const getContext() const { return m_pContext; }
    /** @brief カメラを返す */
    const Camera* const getCamera() const { return m_pCamera; }
    /** @brief トランスフォームを返す */
    const Transform* const getTransform() const { return m_pTransform; }

    // 本来はprivateにすべきだが、このクラスは共通処理をまとめているのでリソースは共有する
    Microsoft::WRL::ComPtr<ID3D11VertexShader>      m_pVertexShader      = nullptr; ///< 頂点シェーダー
    Microsoft::WRL::ComPtr<ID3D11PixelShader>       m_pPixelShader       = nullptr; ///< ピクセルシェーダー
    Microsoft::WRL::ComPtr<ID3D11InputLayout>       m_pInputLayout       = nullptr; ///< 入力レイアウト
    Microsoft::WRL::ComPtr<ID3D11Buffer>            m_pVertexBuffer      = nullptr; ///< 頂点バッファ
    Microsoft::WRL::ComPtr<ID3D11Buffer>            m_pIndexBuffer       = nullptr; ///< インデックスバッファ
    Microsoft::WRL::ComPtr<ID3D11BlendState>        m_pBlendState        = nullptr; ///< ブレンドステート
    Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_pDepthStencilState = nullptr; ///< 深度ステンシルステート
    Microsoft::WRL::ComPtr<ID3D11RasterizerState>   m_pRasterizerState   = nullptr; ///< ラスタライザーステート

private:
    Renderer() = delete; // 必ずRenderContextを渡して初期化
    D3D11_PRIMITIVE_TOPOLOGY   m_topology;            ///< プリミティブトポロジー
    const RenderContext* const m_pContext   = nullptr; ///< レンダリングコンテキスト
    const Camera* const        m_pCamera    = nullptr; ///< カメラ
    const Transform* const     m_pTransform = nullptr; ///< トランスフォーム
};
