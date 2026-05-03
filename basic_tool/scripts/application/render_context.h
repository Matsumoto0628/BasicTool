/**
 * @file render_context.h
 * @brief DirectX11 レンダリングコンテキストクラス
 * @author Matsumoto0628
 * @date 2026-05-03
 */
#pragma once
#include "render_exporter.h"

/**
 * @brief DirectX11 デバイス・スワップチェーン・HDR ポストエフェクトを管理するクラス
 *
 * HDR レンダリング・ブルームエフェクト・スプライトシートエクスポートに対応している。
 */
class RenderContext
{
public:
    /**
     * @brief コンストラクタ
     * @param hWnd レンダリング対象のウィンドウハンドル
     */
    explicit RenderContext(HWND hWnd);
    ~RenderContext();
    /** @brief DirectX11 デバイス・RTV・深度バッファ等を初期化する */
    bool Initialize();
    /** @brief フレームごとの更新処理（エクスポーター更新） */
    void Update();
    /** @brief すべての DirectX リソースを解放する */
    void Finalize();
    /** @brief レンダーターゲットをクリアする */
    void ClearRtv();
    /** @brief HDR 用レンダーターゲットをセットする */
    void SetRtvHDR();
    /** @brief バックバッファ用レンダーターゲットをセットする */
    void SetRtv();
    /** @brief ブルームを含む HDR ポストエフェクトを適用する */
    void PostEffect();
    /** @brief スワップチェーンのバッファを入れ替えて画面に表示する */
    void Swap();
    /** @brief DirectX11 デバイスを返す */
    ID3D11Device* GetDevice() const { return m_pDevice.Get(); }
    /** @brief DirectX11 デバイスコンテキストを返す */
    ID3D11DeviceContext* GetDeviceContext() const { return m_pDeviceContext.Get(); }
    /** @brief レンダリング幅（ピクセル）を返す */
    unsigned int GetWidth() const { return m_screenWidth; }
    /** @brief レンダリング高さ（ピクセル）を返す */
    unsigned int GetHeight() const { return m_screenHeight; }
    /** @brief エクスポート用テクスチャを返す */
    ID3D11Texture2D* GetExportTexture() const { return m_pExportTexture.Get(); }
    /**
     * @brief スプライトシートエクスポートを開始する
     * @param sheetName 出力ファイル名（拡張子なし）
     * @param numW      横方向のフレーム数
     * @param numH      縦方向のフレーム数
     */
    void Export(std::wstring sheetName, UINT numW, UINT numH) { m_exporter.Export(sheetName, numW, numH); }

private:
    bool initDeviceAndSwapChain();
    bool initBackBuffer();
    bool initDepthStencilView();
    void initViewPort();
    bool initHDR();
    bool initBloom();
    bool initConstantBufferBlur();
    bool initVertexShader();
    bool initPixelShader();
    bool initSampler();
    void updateConstantBufferBlur(const Vec2& dir);
    void drawFullscreen();
    bool initExport();

    struct ConstantBufferBlur // Vecは使えない
    {
        float texelSize[2];
        float dir[2];
    };

    static const UINT BACK_BUFFER_NUM    = 3;
    static const UINT FEATURE_LEVELS_NUM = 2;
    static const Vec4 BACK_BUFFER_COLOR;

    Microsoft::WRL::ComPtr<ID3D11Device>             m_pDevice                    = nullptr;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext>      m_pDeviceContext             = nullptr;
    Microsoft::WRL::ComPtr<IDXGISwapChain>           m_pSwapChain                = nullptr;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView>   m_pRenderTargetView         = nullptr;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView>   m_pDepthStencilView         = nullptr;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView>   m_pRenderTargetViewHDR      = nullptr;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pShaderResourceViewHDR    = nullptr;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView>   m_pRenderTargetViewBloomA   = nullptr;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView>   m_pRenderTargetViewBloomB   = nullptr;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pShaderResourceViewBloomA = nullptr;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pShaderResourceViewBloomB = nullptr;
    Microsoft::WRL::ComPtr<ID3D11VertexShader>       m_pVertexShaderFullscreen   = nullptr;
    Microsoft::WRL::ComPtr<ID3D11PixelShader>        m_pPixelShaderExtract       = nullptr;
    Microsoft::WRL::ComPtr<ID3D11PixelShader>        m_pPixelShaderBlur          = nullptr;
    Microsoft::WRL::ComPtr<ID3D11PixelShader>        m_pPixelShaderComposite     = nullptr;
    Microsoft::WRL::ComPtr<ID3D11SamplerState>       m_pSamplerLinear            = nullptr;
    Microsoft::WRL::ComPtr<ID3D11Buffer>             m_pConstantBufferBlur       = nullptr;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView>   m_pRenderTargetViewExport   = nullptr;
    Microsoft::WRL::ComPtr<ID3D11Texture2D>          m_pExportTexture            = nullptr;
    D3D11_VIEWPORT     m_viewPort[1];
    D3D11_VIEWPORT     m_viewPortBloom[1];
    D3D_FEATURE_LEVEL  m_pFeatureLevels[FEATURE_LEVELS_NUM] = {};
    D3D_FEATURE_LEVEL  m_featureLevelsSupported;
    UINT               m_screenWidth  = 0;
    UINT               m_screenHeight = 0;
    HWND               m_hWnd         = nullptr;
    RenderExporter     m_exporter;
};
